/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/itoserver.h"
#include <errno.h>
#include "bu/serversocket.h"
#include "bu/client.h"
#include "bu/socket.h"
#include "osx_compatibility.h"

Bu::ItoServer::ItoServer() :
	nTimeoutSec( 1 ),
	nTimeoutUSec( 0 )
{
	FD_ZERO( &fdActive );
}

Bu::ItoServer::~ItoServer()
{
	while( !qClientCleanup.isEmpty() )
	{
		ItoClient *pCli = qClientCleanup.dequeue();
		pCli->join();
		delete pCli;
	}
	// TODO:  Make sure here that each client has shutdown it's socket, and
	// maybe even written any extra data, we could put a timelimit on this...
	// anyway, it's not as clean as it could be right now.
	for( ClientHash::iterator i = hClients.begin(); i != hClients.end(); i++ )
	{
		ItoClient *pCli = (*i);
		pCli->join();
		delete pCli;
	}
}

void Bu::ItoServer::addPort( int nPort, int nPoolSize )
{
	ServerSocket *s = new ServerSocket( nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::ItoServer::addPort( const FString &sAddr, int nPort, int nPoolSize )
{
	ServerSocket *s = new ServerSocket( sAddr, nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::ItoServer::setTimeout( int nTimeoutSec, int nTimeoutUSec )
{
	this->nTimeoutSec = nTimeoutSec;
	this->nTimeoutUSec = nTimeoutUSec;
}

void Bu::ItoServer::addClient( int nSocket, int nPort )
{
	ItoClient *pC = new ItoClient( *this, nSocket, nPort, nTimeoutSec,
			nTimeoutUSec );

	imClients.lock();
	hClients.insert( nSocket, pC );
	imClients.unlock();
	
	pC->start();
}

void *Bu::ItoServer::run()
{
	for(;;)
	{
		struct timeval xTimeout = { nTimeoutSec, nTimeoutUSec };
		
		fd_set fdRead = fdActive;
		fd_set fdWrite = fdActive;
		fd_set fdException = fdActive;

		if( TEMP_FAILURE_RETRY( select( FD_SETSIZE, &fdRead, NULL, &fdException, &xTimeout ) ) < 0 )
		{
			throw ExceptionBase("Error attempting to scan open connections.");
		}

		for( ServerHash::iterator i = hServers.begin(); i != hServers.end(); i++ )
		{
			if( FD_ISSET( i.getKey(), &fdRead ) )
			{
				ServerSocket *pSrv = i.getValue();
				addClient( pSrv->accept(), pSrv->getPort() );
			}
		}

		while( !qClientCleanup.isEmpty() )
		{
			ItoClient *pCli = qClientCleanup.dequeue();
			pCli->join();
			delete pCli;
		}
	}

	return NULL;
}

void Bu::ItoServer::clientCleanup( int iSocket )
{
	imClients.lock();
	ItoClient *pCli = hClients.get( iSocket );
	imClients.unlock();
	qClientCleanup.enqueue( pCli );
}

Bu::ItoServer::ItoClient::ItoClient( ItoServer &rSrv, int iSocket, int iPort,
		int nTimeoutSec, int nTimeoutUSec ) :
	rSrv( rSrv ),
	iSocket( iSocket ),
	iPort( iPort ),
	nTimeoutSec( nTimeoutSec ),
	nTimeoutUSec( nTimeoutUSec )
{
	FD_ZERO( &fdActive );
	FD_SET( iSocket, &fdActive );

	pClient = new Client(
		new Bu::Socket( iSocket )
		);
}

Bu::ItoServer::ItoClient::~ItoClient()
{
}

void *Bu::ItoServer::ItoClient::run()
{
	rSrv.onNewConnection( pClient, iPort );

	for(;;)
	{
		struct timeval xTimeout = { nTimeoutSec, nTimeoutUSec };
		
		fd_set fdRead = fdActive;
		fd_set fdException = fdActive;

		if( TEMP_FAILURE_RETRY( select( FD_SETSIZE, &fdRead, NULL, &fdException, &xTimeout ) ) < 0 )
		{
			throw ExceptionBase("Error attempting to scan open connections.");
		}

		if( FD_ISSET( iSocket, &fdRead ) )
		{
			pClient->processInput();
			if( !pClient->isOpen() )
			{
				rSrv.onClosedConnection( pClient );

				rSrv.clientCleanup( iSocket );
				return NULL;
			}
		}

		// Now we just try to write all the pending data on the socket.
		// this could be done better eventually, if we care about the socket
		// wanting to accept writes (using a select).
		pClient->processOutput();
	}

	return NULL;
}

