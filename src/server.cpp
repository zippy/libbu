/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/server.h"
#include <errno.h>
#include <unistd.h>
#include "bu/tcpserversocket.h"
#include "bu/client.h"
#include "bu/tcpsocket.h"
#include "bu/config.h"

Bu::Server::Server() :
	nTimeoutSec( 0 ),
	nTimeoutUSec( 0 ),
	bAutoTick( false )
{
	FD_ZERO( &fdActive );
}

Bu::Server::~Server()
{
	shutdown();
}

void Bu::Server::addPort( int nPort, int nPoolSize )
{
	TcpServerSocket *s = new TcpServerSocket( nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::Server::addPort( const FString &sAddr, int nPort, int nPoolSize )
{
	TcpServerSocket *s = new TcpServerSocket( sAddr, nPort, nPoolSize );
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
				TcpServerSocket *pSrv = hServers.get( j );
				addClient( pSrv->accept(), pSrv->getPort() );
			}
			else
			{
				Client *pClient = hClients.get( j );
				pClient->processInput();
				if( !pClient->isOpen() )
				{
					closeClient( j );
				}
			}
		}
		if( FD_ISSET( j, &fdWrite ) )
		{
			try
			{
				Client *pClient = hClients.get( j );
				try
				{
					pClient->processOutput();
				}
				catch( Bu::TcpSocketException &e )
				{
					closeClient( j );
				}
			}
			catch( Bu::HashException &e )
			{
				// Do nothing, I guess, the client is already dead...
				// TODO:  Someday, we may want to handle this more graceully.
			}
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
		closeClient( *i );
	}

	if( bAutoTick )
		tick();
}

void Bu::Server::addClient( int nSocket, int nPort )
{
	FD_SET( nSocket, &fdActive );

	Client *c = new Client(
		new Bu::TcpSocket( nSocket ),
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

void Bu::Server::shutdown()
{
	for( SrvHash::iterator i = hServers.begin(); i != hServers.end(); i++ )
	{
		delete *i;
	}

	hServers.clear();

	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		closeClient( i.getKey() );
	}

	hClients.clear();
}

void Bu::Server::closeClient( int iSocket )
{
	Bu::Client *pClient = hClients.get( iSocket );
	onClosedConnection( pClient );
	pClient->close();
	hClients.erase( iSocket );
	FD_CLR( iSocket, &fdActive );
	delete pClient;
}

