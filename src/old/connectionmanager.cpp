#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "connectionmanager.h"
#include <fcntl.h>

ConnectionManager::ConnectionManager( int nInitPool ) :
	xLog( MultiLog::getInstance() )
{
	//nMasterSocket = -1;
	pMonitor = NULL;
	for( int j = 0; j < nInitPool; j++ )
	{
		lInactive.insert( lInactive.begin(), new Connection() );
	}
	FD_ZERO (&fdActive);
	FD_ZERO (&fdRead);
	FD_ZERO (&fdWrite);
	FD_ZERO (&fdException);
}

ConnectionManager::~ConnectionManager()
{
	std::list<Connection *>::const_iterator i;
	for( i = lActive.begin(); i != lActive.end(); i++ )
	{
		delete (*i);
	}
	for( i = lInactive.begin(); i != lInactive.end(); i++ )
	{
		delete (*i);
	}
}

bool ConnectionManager::startServer( int nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons( nPort );

	// I think this specifies who we will accept connections from,
	// a good thing to make configurable later on
	name.sin_addr.s_addr = htonl( INADDR_ANY );

	return startServer( name );
}

bool ConnectionManager::startServer( const char *sAddr, int nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons( nPort );

	inet_aton( sAddr, &name.sin_addr );

	return startServer( name );
}

bool ConnectionManager::startServer( struct sockaddr_in &name )
{
	/* Create the socket. */
	int nMasterSocket = socket (PF_INET, SOCK_STREAM, 0);
	if (nMasterSocket < 0)
	{
		xLog.LineLog( MultiLog::LError, "Couldn't create a listen socket.");
		return false;
	}

	int opt = 1;
	setsockopt(
		nMasterSocket,
		SOL_SOCKET,
		SO_REUSEADDR,
		(char *)&opt,
		sizeof(opt)
		);

	if (bind (nMasterSocket, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		xLog.LineLog( MultiLog::LError, "Couldn't bind to the listen socket.");
		return false;
	}

	if (listen (nMasterSocket, 40) < 0)
	{
		xLog.LineLog( MultiLog::LError, "Couldn't begin listening to the server socket.");
		return false;
	}

	/* Initialize the set of active sockets. */
	FD_SET (nMasterSocket, &fdActive);

	sMasterSocket[nMasterSocket] = name.sin_port;

	return true;
}

bool ConnectionManager::startServer( int nPort, int nNumTries, int nTimeout )
{
	struct timeval xTimeout;

	for( int j = 0; j < nNumTries; j++ )
	{
		xLog.LineLog( MultiLog::LStatus, "Attempting to create server socket (attempt [%d/%d])...", j+1, nNumTries );
		if( startServer( nPort ) == true )
		{
			return true;
		}
		else if( j < nNumTries-1 )
		{
			xLog.LineLog( MultiLog::LStatus, "Waiting for %d secconds to allow port to clear...", nTimeout );
			xTimeout.tv_sec = nTimeout;
			xTimeout.tv_usec = 0;
			if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &xTimeout) < 0) {
				xLog.LineLog( MultiLog::LError, "Error using select to sleep for a while.");
			}
			usleep( nTimeout );
		}
	}
	
	return false;
}

bool ConnectionManager::scanConnections( int nTimeout, bool bForceTimeout )
{
	struct timeval xTimeout;

	xTimeout.tv_sec = nTimeout / 1000000;
	xTimeout.tv_usec = nTimeout % 1000000;

	/* Block until input arrives on one or more active sockets. */
	fdRead = fdActive;
	fdWrite = fdActive;
	fdException = fdActive;

	// We removed the write checking because it just checks to see if you *can*
	// write...that's stupid, they're all open, so it always exits immediately
	// if there are ANY connections there...
	if( TEMP_FAILURE_RETRY( select( FD_SETSIZE, &fdRead, (fd_set *)0/*&fdWrite*/, &fdException, &xTimeout ) ) < 0 )
	{
		xLog.LineLog( MultiLog::LError, "Error attempting to scan open connections.");
		perror("ConnectionManager");
		return false;
	}
	// Now we use select to sleep as well as to scan for connections, now we
	// just need to fix the fact that if there are no connections, the seccond
	// select call doesn't return until there is a connection...
	if( bForceTimeout )
	{
		if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &xTimeout) < 0) {
			xLog.LineLog( MultiLog::LError, "Error using select to sleep for a while.");
		}
	}

	/* Service all the sockets with input pending. */
	for( int i = 0; i < FD_SETSIZE; ++i )
	{
		if( FD_ISSET( i, &fdRead ) )
		{
			if( sMasterSocket.find( i ) != sMasterSocket.end() )
			{
				addConnection( i );
			}
			else
			{
				Connection *pCon = findActiveConnection( i );
				if( pCon == NULL )
				{
					xLog.LineLog( MultiLog::LError, "A connection object was lost, or never created!");
					return false;
				}

				/* Data arriving on an already-connected socket. */
				if( pCon->readInput() == 0 )
				{
					xLog.LineLog( MultiLog::LStatus, "Closing connection due to disconnect.");
					close( i );
					FD_CLR( i, &fdActive );
					pMonitor->onClosedConnection( pCon );
					pCon->close();
				}
				else
				{
					// We actually read something...but the connection handles
					// protocol notification, so we don't need to do anything
					// here...
				}
			}
		}
	}
	std::list<Connection *>::iterator i;
	for( i = lActive.begin(); i != lActive.end(); i++ )
	{
		if( (*i)->isActive() == false )
		{
			std::list<Connection *>::iterator l = i;
			i--;
			lInactive.insert( lInactive.end(), *l );
			lActive.erase( l );
			continue;
		}
		(*i)->getProtocol()->poll();
		if( (*i)->hasOutput() )
		{
			(*i)->writeOutput();
		}
		if( (*i)->needDisconnect() && !(*i)->hasOutput() )
		{
			int prt = (*i)->getSocket();
			close( prt );
			FD_CLR( prt, &fdActive );
			pMonitor->onClosedConnection( *i );
			(*i)->close();
			lInactive.insert( lInactive.end(), *i );
			std::list<Connection *>::iterator l = i;
			i--;
			lActive.erase( l );
			xLog.LineLog( MultiLog::LStatus, "Closing connection due to server request.");
		}
	}

	return true;
}

bool ConnectionManager::shutdownServer()
{
	while( !lActive.empty() )
	{
		Connection *i = *(lActive.begin());
		if( i->isActive() )
		{
			pMonitor->onClosedConnection( i );
			i->close();
			lInactive.insert( lInactive.end(), i );
			lActive.erase( lActive.begin() );
		}
	}
/*
	for( int i = 0; i < nPoolSize; i++ )
	{

		int prt = axConPool[i].getSocket();
		close( prt );
//		FD_CLR( prt, &fdActive );
		pMonitor->onClosedConnection( &axConPool[i] );
		axConPool[i].close();
	}
*/
	std::map<int,int>::iterator i;
	for( i = sMasterSocket.begin(); i != sMasterSocket.end(); i++ )
	{
		int nSocket = (*i).first;
		shutdown( nSocket, SHUT_RDWR );
		close( nSocket );
	}

	return true;
}

bool ConnectionManager::broadcastMessage( const char *lpData, int nExcludeSocket )
{
	std::list<Connection *>::const_iterator i;
	for( i = lActive.begin(); i != lActive.end(); i++ )
	{
		if( (*i)->isActive() &&
			(*i)->getSocket() != nExcludeSocket )
		{
			(*i)->appendOutput( lpData );
		}
	}

	return true;
}

bool ConnectionManager::addConnection( int nSocket )
{
	struct sockaddr_in clientname;
	size_t size;
	int newSocket;

	size = sizeof( clientname );
#ifdef __CYGWIN__
	newSocket = accept( nSocket, (struct sockaddr *) &clientname, (int *)&size );
#else
	newSocket = accept( nSocket, (struct sockaddr *) &clientname, &size );
#endif
	if( newSocket < 0 )
	{
		xLog.LineLog( MultiLog::LError, "Error accepting a new connection!" );
		return false;
	}
//	char *tmpa = inet_ntoa(clientname.sin_addr);
	char tmpa[20];
	inet_ntop( AF_INET, (void *)&clientname.sin_addr, tmpa, 20 );
	xLog.LineLog( MultiLog::LStatus, "New connection from host %s, port %hd.", tmpa, ntohs (clientname.sin_port) );
/*
	int nCnt = 0;
	for( int j = 0; j < nPoolSize; j++ )
	{
		if( axConPool[j].isActive() )
		{
			nCnt++;
		}
	}
	xLog.LineLog( MultiLog::LStatus, "Connections %d/%d.", nCnt, nPoolSize );
	*/
//	free( tmpa );
	FD_SET( newSocket, &fdActive );

	//void nonblock(socket_t s)
	{
		int flags;

		flags = fcntl(newSocket, F_GETFL, 0);
		flags |= O_NONBLOCK;
		if (fcntl(newSocket, F_SETFL, flags) < 0)
		{
			return false;
		}
	}

	Connection *pCon = getInactiveConnection();
	pCon->open( newSocket );

	pMonitor->onNewConnection( pCon, (*sMasterSocket.find(nSocket)).second );
	if( pCon->getProtocol() )
		pCon->getProtocol()->onNewConnection();

	lActive.insert( lActive.end(), pCon );

	return true;
}

void ConnectionManager::connect(
	const char *lpAddress,
	int nPort,
	int nProtocolPort,
	Protocol *pNewProto
	)
{
	Connection *pCon = getInactiveConnection();
	pCon->open( lpAddress, nPort );
	int nSocket = pCon->getSocket();
	FD_SET( nSocket, &fdActive );

	pCon->setProtocol( pNewProto );
	pMonitor->onNewClientConnection( pCon, nProtocolPort );
	if( pCon->getProtocol() )
		pCon->getProtocol()->onNewClientConnection();

	lActive.insert( lActive.end(), pCon );
}

Connection *ConnectionManager::getInactiveConnection()
{
	if( lInactive.empty() )
	{
		return new Connection();
	}
	Connection *pCon = *(lInactive.begin());
	lInactive.erase( lInactive.begin() );
	return pCon;
}

Connection *ConnectionManager::findActiveConnection( int nSocket )
{
	std::list<Connection *>::const_iterator i;
	for( i = lActive.begin(); i != lActive.end(); i++ )
	{
		if( (*i)->getSocket() == nSocket )
		{
			return *i;
		}
	}

	return NULL;
}

void ConnectionManager::setConnectionMonitor( ConnectionMonitor *pNewMonitor )
{
	pMonitor = pNewMonitor;
}
