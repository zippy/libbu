/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/itoserver.h"
#include <errno.h>
#include "bu/tcpserversocket.h"
#include "bu/client.h"
#include "bu/tcpsocket.h"

#include "bu/config.h"

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
	TcpServerSocket *s = new TcpServerSocket( nPort, nPoolSize );
	int nSocket = s->getSocket();
	FD_SET( nSocket, &fdActive );
	hServers.insert( nSocket, s );
}

void Bu::ItoServer::addPort( const String &sAddr, int nPort, int nPoolSize )
{
	TcpServerSocket *s = new TcpServerSocket( sAddr, nPort, nPoolSize );
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

void Bu::ItoServer::run()
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
				TcpServerSocket *pSrv = i.getValue();
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
		new Bu::TcpSocket( iSocket ),
		new SrvClientLinkFactory( rSrv )
		);
}

Bu::ItoServer::ItoClient::~ItoClient()
{
}

void Bu::ItoServer::ItoClient::run()
{
	imProto.lock();
	rSrv.onNewConnection( pClient, iPort );
	pClient->processOutput();
	imProto.unlock();

	for(;;)
	{
		struct timeval xTimeout = { nTimeoutSec, nTimeoutUSec };
		
		fd_set fdRead = fdActive;
		fd_set fdWrite;
		fd_set fdException = fdActive;

		FD_ZERO( &fdWrite );
		if( pClient->hasOutput() )
			FD_SET( iSocket, &fdWrite );

		if( TEMP_FAILURE_RETRY( select( FD_SETSIZE,
				&fdRead, &fdWrite, &fdException, &xTimeout ) ) < 0 )
		{
			throw ExceptionBase("Error attempting to scan open connections.");
		}

		while( !qMsg.isEmpty() )
		{
			imProto.lock();
			Bu::String *pMsg = qMsg.dequeue();
			pClient->onMessage( *pMsg );
			delete pMsg;
			pClient->processOutput();
			imProto.unlock();
		}

		if( FD_ISSET( iSocket, &fdRead ) )
		{
			imProto.lock();
			pClient->processInput();
			imProto.unlock();
			if( !pClient->isOpen() )
			{
				imProto.lock();
				rSrv.onClosedConnection( pClient );
				imProto.unlock();

				rSrv.clientCleanup( iSocket );
				
				return;
			}
		}

		if( FD_ISSET( iSocket, &fdWrite ) )
		{
			imProto.lock();
			pClient->processOutput();
			imProto.unlock();
		}
	}
}

Bu::ItoServer::SrvClientLink::SrvClientLink( ItoClient *pClient ) :
	pClient( pClient )
{
}

Bu::ItoServer::SrvClientLink::~SrvClientLink()
{
}

void Bu::ItoServer::SrvClientLink::sendMessage( const Bu::String &sMsg )
{
	if( !pClient->imProto.trylock() )
	{
		pClient->pClient->onMessage( sMsg );
		pClient->pClient->processOutput();
		pClient->imProto.unlock();
	}
	else
	{
		Bu::String *pMsg = new Bu::String( sMsg );
		pClient->qMsg.enqueue( pMsg );
	}
}

Bu::ItoServer::SrvClientLinkFactory::SrvClientLinkFactory(
		Bu::ItoServer &rSrv ) :
	rSrv( rSrv )
{
}

Bu::ItoServer::SrvClientLinkFactory::~SrvClientLinkFactory()
{
}

Bu::ClientLink *Bu::ItoServer::SrvClientLinkFactory::createLink(
		Bu::Client *pClient )
{
	rSrv.imClients.lock();
	ItoClient *pCli = rSrv.hClients.get( *pClient->getSocket() );
	rSrv.imClients.unlock();

	return new SrvClientLink( pCli );
}

