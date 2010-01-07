/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/server.h"
#include <errno.h>
#include <unistd.h>
#include "bu/serversocket.h"
#include "bu/client.h"
#include "bu/socket.h"
#include "bu/osx_compatibility.h"

Bu::Server::Server() :
	nTimeoutSec( 0 ),
	nTimeoutUSec( 0 ),
	bAutoTick( false )
{
	FD_ZERO( &fdActive );
}

Bu::Server::~Server()
{
	for( SrvHash::iterator i = hServers.begin(); i != hServers.end(); i++ )
	{
		delete *i;
	}

	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		delete *i;
	}
}

void Bu::Server::addPort( int nPort, int nPoolSize )
{
	ServerSocket *s = new ServerSocket( nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::Server::addPort( const FString &sAddr, int nPort, int nPoolSize )
{
	ServerSocket *s = new ServerSocket( sAddr, nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::Server::setTimeout( int nTimeoutSec, int nTimeoutUSec )
{
	this->nTimeoutSec = nTimeoutSec;
	this->nTimeoutUSec = nTimeoutUSec;
}

void Bu::Server::scan()
{
	struct timeval xTimeout = { nTimeoutSec, nTimeoutUSec };
	
	fd_set fdRead = fdActive;
	fd_set fdWrite /* = fdActive*/;
	fd_set fdException = fdActive;

	FD_ZERO( &fdWrite );
	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		if( (*i)->hasOutput() )
			FD_SET( i.getKey(), &fdWrite );
	}

	if( TEMP_FAILURE_RETRY( select( FD_SETSIZE,
			&fdRead, &fdWrite, &fdException, &xTimeout ) ) < 0 )
	{
		throw ExceptionBase("Error attempting to scan open connections.");
	}

	for( int j = 0; j < FD_SETSIZE; j++ )
	{
		if( FD_ISSET( j, &fdRead ) )
		{
			if( hServers.has( j ) )
			{
				ServerSocket *pSrv = hServers.get( j );
				addClient( pSrv->accept(), pSrv->getPort() );
			}
			else
			{
				Client *pClient = hClients.get( j );
				pClient->processInput();
				if( !pClient->isOpen() )
				{
					onClosedConnection( pClient );
					pClient->close();
					hClients.erase( j );
					FD_CLR( j, &fdActive );
				}
			}
		}
		if( FD_ISSET( j, &fdWrite ) )
		{
			Client *pClient = hClients.get( j );
			pClient->processOutput();
		}
	}

	Bu::List<int> lDelete;
	// Now we just try to write all the pending data on all the sockets.
	// this could be done better eventually, if we care about the socket
	// wanting to accept writes (using a select).
	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		if( (*i)->wantsDisconnect() && !(*i)->hasOutput() )
		{
			lDelete.append( i.getKey() );
		}
	}

	for( Bu::List<int>::iterator i = lDelete.begin(); i != lDelete.end(); i++ )
	{
		Client *pClient = hClients.get( *i );
		onClosedConnection( pClient );
		pClient->close();
		hClients.erase( *i );
		FD_CLR( *i, &fdActive );
	}

	if( bAutoTick )
		tick();
}

void Bu::Server::addClient( int nSocket, int nPort )
{
	FD_SET( nSocket, &fdActive );

	Client *c = new Client(
		new Bu::Socket( nSocket ),
		new SrvClientLinkFactory()
		);
	hClients.insert( nSocket, c );

	onNewConnection( c, nPort );
}

Bu::Server::SrvClientLink::SrvClientLink( Bu::Client *pClient ) :
	pClient( pClient )
{
}

Bu::Server::SrvClientLink::~SrvClientLink()
{
}

void Bu::Server::SrvClientLink::sendMessage( const Bu::FString &sMsg )
{
	pClient->onMessage( sMsg );
}

Bu::Server::SrvClientLinkFactory::SrvClientLinkFactory()
{
}

Bu::Server::SrvClientLinkFactory::~SrvClientLinkFactory()
{
}

Bu::ClientLink *Bu::Server::SrvClientLinkFactory::createLink(
		Bu::Client *pClient )
{
	return new SrvClientLink( pClient );
}

void Bu::Server::setAutoTick( bool bEnable )
{
	bAutoTick = bEnable;
}

void Bu::Server::tick()
{
	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		(*i)->tick();
	}
}

