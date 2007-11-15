/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
#include <fcntl.h>
#include "serversocket.h"
#include "exceptions.h"
#include "osx_compatibility.h"

Bu::ServerSocket::ServerSocket( int nPort, int nPoolSize ) :
	nPort( nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons( nPort );

	// I think this specifies who we will accept connections from,
	// a good thing to make configurable later on
	name.sin_addr.s_addr = htonl( INADDR_ANY );

	startServer( name, nPoolSize );
}

Bu::ServerSocket::ServerSocket(const FString &sAddr,int nPort, int nPoolSize) :
	nPort( nPort )
{
	/* Create the socket and set it up to accept connections. */
	struct sockaddr_in name;

	/* Give the socket a name. */
	name.sin_family = AF_INET;
	name.sin_port = htons( nPort );

	inet_aton( sAddr.getStr(), &name.sin_addr );

	startServer( name, nPoolSize );
}

Bu::ServerSocket::~ServerSocket()
{
}

void Bu::ServerSocket::startServer( struct sockaddr_in &name, int nPoolSize )
{
	/* Create the socket. */
	nServer = socket( PF_INET, SOCK_STREAM, 0 );
	if( nServer < 0 )
	{
		throw Bu::SocketException("Couldn't create a listen socket.");
	}

	int opt = 1;
	setsockopt(
		nServer,
		SOL_SOCKET,
		SO_REUSEADDR,
		(char *)&opt,
		sizeof( opt )
		);

	if( bind( nServer, (struct sockaddr *) &name, sizeof(name) ) < 0 )
	{
		throw Bu::SocketException("Couldn't bind to the listen socket.");
	}

	if( listen( nServer, nPoolSize ) < 0 )
	{
		throw Bu::SocketException(
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

	if( TEMP_FAILURE_RETRY(select( nServer+1, &fdRead, NULL, NULL, &xT )) < 0 )
	{
		throw SocketException(
			"Error scanning for new connections: %s", strerror( errno )
			);
	}

	if( FD_ISSET( nServer, &fdRead ) )
	{
		struct sockaddr_in clientname;
		size_t size;
		int nClient;

		size = sizeof( clientname );
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
		if( nClient < 0 )
		{
			throw SocketException(
				"Error accepting a new connection: %s", strerror( errno )
				);
		}
		char tmpa[20];
		inet_ntop( AF_INET, (void *)&clientname.sin_addr, tmpa, 20 );
		//"New connection from host %s, port %hd.",
		//  tmpa, ntohs (clientname.sin_port) );
		
		{
			int flags;

			flags = fcntl( nClient, F_GETFL, 0 );
			flags |= O_NONBLOCK;
			if( fcntl( nClient, F_SETFL, flags ) < 0)
			{
				throw SocketException(
					"Error setting option on client socket: %s",
					strerror( errno )
					);
			}
		}

		return nClient;
	}

	return -1;
}

int Bu::ServerSocket::getPort()
{
	return nPort;
}

