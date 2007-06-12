#include "server.h"
#include <errno.h>

Bu::Server::Server() :
	nTimeoutSec( 0 ),
	nTimeoutUSec( 0 )
{
	FD_ZERO( &fdActive );
}

Bu::Server::~Server()
{
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
	fd_set fdWrite = fdActive;
	fd_set fdException = fdActive;

	if( TEMP_FAILURE_RETRY( select( FD_SETSIZE, &fdRead, NULL, &fdException, &xTimeout ) ) < 0 )
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
				
			}
		}
	}
}

void Bu::Server::addClient( int nSocket, int nPort )
{
	FD_SET( nSocket, &fdActive );

	Client *c = new Client();
	hClients.insert( nSocket, c );

	onNewConnection( c, nPort );
}

