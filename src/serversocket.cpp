/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef WIN32
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <arpa/inet.h>
#endif

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
//#include <termios.h>
#include <fcntl.h>
#include "bu/serversocket.h"
#include "bu/osx_compatibility.h"
#include "bu/win32_compatibility.h"
#include "bu/linux_compatibility.h"

namespace Bu { subExceptionDef( ServerSocketException ) }

Bu::ServerSocket::ServerSocket( int nPort, int nPoolSize ) :
	nPort( nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = bu_htons( nPort );

	// I think this specifies who we will accept connections from,
	// a good thing to make configurable later on
	name.sin_addr.s_addr = bu_htonl( INADDR_ANY );

	startServer( name, nPoolSize );
}

Bu::ServerSocket::ServerSocket(const FString &sAddr,int nPort, int nPoolSize) :
	nPort( nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;

	name.sin_port = bu_htons( nPort );

#ifdef WIN32
	name.sin_addr.s_addr = bu_inet_addr( sAddr.getStr() );
#else
	inet_aton( sAddr.getStr(), &name.sin_addr );
#endif

	startServer( name, nPoolSize );
}

Bu::ServerSocket::ServerSocket( int nServer, bool bInit, int nPoolSize ) :
	nServer( nServer ),
	nPort( 0 )
{
	if( bInit )
	{
		struct sockaddr name;
		socklen_t namelen = sizeof(name);
		getpeername( nServer, &name, &namelen );

		initServer( *((sockaddr_in *)&name), nPoolSize );
	}
	else
	{
		FD_ZERO( &fdActive );
		FD_SET( nServer, &fdActive );
	}
}

Bu::ServerSocket::ServerSocket( const ServerSocket &rSrc )
{
	nServer = dup( rSrc.nServer );
	nPort = rSrc.nPort;
	FD_ZERO( &fdActive );
	FD_SET( nServer, &fdActive );
}

Bu::ServerSocket::~ServerSocket()
{
	if( nServer > -1 )
		::close( nServer );
}

void Bu::ServerSocket::startServer( struct sockaddr_in &name, int nPoolSize )
{
	/* Create the socket. */
	nServer = bu_socket( PF_INET, SOCK_STREAM, 0 );

	if( nServer < 0 )
	{
		throw Bu::ServerSocketException("Couldn't create a listen socket.");
	}
	
	int opt = 1;
	bu_setsockopt(
			nServer,
			SOL_SOCKET,
			SO_REUSEADDR,
			(char *)&opt,
			sizeof( opt )
			);

	initServer( name, nPoolSize );
}

void Bu::ServerSocket::initServer( struct sockaddr_in &name, int nPoolSize )
{
	if( bu_bind( nServer, (struct sockaddr *) &name, sizeof(name) ) < 0 )
	{
		throw Bu::ServerSocketException("Couldn't bind to the listen socket.");
	}

	if( bu_listen( nServer, nPoolSize ) < 0 )
	{
		throw Bu::ServerSocketException(
			"Couldn't begin listening to the server socket."
			);
	}

	FD_ZERO( &fdActive );
	/* Initialize the set of active sockets. */
	FD_SET( nServer, &fdActive );
}

int Bu::ServerSocket::getSocket()
{
	return nServer;
}

int Bu::ServerSocket::accept( int nTimeoutSec, int nTimeoutUSec )
{
	fd_set fdRead = fdActive;

	struct timeval xT;

	xT.tv_sec = nTimeoutSec;
	xT.tv_usec = nTimeoutUSec;	

	if( TEMP_FAILURE_RETRY(
		bu_select( nServer+1, &fdRead, NULL, NULL, &xT )) < 0 )
	{
		throw Bu::ServerSocketException(
			"Error scanning for new connections: %s", strerror( errno )
			);
	}

	if( FD_ISSET( nServer, &fdRead ) )
	{
		struct sockaddr_in clientname;
		socklen_t size;
		int nClient;

		size = sizeof( clientname );
#ifdef WIN32
		nClient = bu_accept( nServer, (struct sockaddr *)&clientname, &size);
#else /* not-WIN32 */
#ifdef __CYGWIN__
		nClient = ::accept( nServer, (struct sockaddr *)&clientname,
				(int *)&size
				);
#else /* not-cygwin */
#ifdef __APPLE__
  		nClient = ::accept( nServer, (struct sockaddr *)&clientname, (socklen_t*)&size );
#else /* linux */
  		nClient = ::accept( nServer, (struct sockaddr *)&clientname, &size );
#endif /* __APPLE__ */
#endif /* __CYGWIN__ */
#endif /* WIN32 */
		if( nClient < 0 )
		{
			throw Bu::ServerSocketException(
				"Error accepting a new connection: %s", strerror( errno )
				);
		}

#ifndef WIN32
		char tmpa[20];
		inet_ntop( AF_INET, (void *)&clientname.sin_addr, tmpa, 20 );
		//"New connection from host %s, port %hd.",
		//  tmpa, ntohs (clientname.sin_port) );
#endif
		
		{
#ifndef WIN32
			int flags;
			flags = fcntl( nClient, F_GETFL, 0 );
			flags |= O_NONBLOCK;
			if( fcntl( nClient, F_SETFL, flags ) < 0)
			{
				throw Bu::ServerSocketException(
					"Error setting option on client socket: %s",
					strerror( errno )
					);
			}
#else
			//-------------------------
			// Set the socket I/O mode: In this case FIONBIO
			// enables or disables the blocking mode for the 
			// socket based on the numerical value of iMode.
			// If iMode = 0, blocking is enabled; 
			// If iMode != 0, non-blocking mode is enabled.
			u_long iMode = 1;
			bu_ioctlsocket(nClient, FIONBIO, &iMode);			
#endif
		}

		return nClient;
	}

	return -1;
}

int Bu::ServerSocket::getPort()
{
	return nPort;
}

