/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SERVER_SOCKET_H
#define BU_SERVER_SOCKET_H

#include <stdint.h>
#include "bu/fstring.h"
#include "bu/exceptionbase.h"
#include <sys/select.h>

namespace Bu
{
	subExceptionDecl( ServerSocketException );

	/**
	 * A single tcp/ip server socket.  When created the server socket will bind
	 * to the specified interface and port, and immediately begin listening for
	 * connections.  When connections come in they are pooled by the networking
	 * drivers in the kernel until they are accepted, this means that failure
	 * to keep space in the connection pool will result in connection refusals.
	 *
	 * Although the accept function returns an integral file descriptor, it is
	 * designed to be used with the Socket class.
	 *
	 *@author Mike Buland
	 *@ingroup Serving
	 */
	class ServerSocket
	{
	public:
		ServerSocket( int nPort, int nPoolSize=40 );
		ServerSocket( const FString &sAddr, int nPort, int nPoolSize=40 );
		virtual ~ServerSocket();

		int accept( int nTimeoutSec=0, int nTimeoutUSec=0 );
		int getSocket();
		int getPort();

	private:
		void startServer( struct sockaddr_in &name, int nPoolSize );

		fd_set fdActive;
		int nServer;
		int nPort;
	};
}

#endif
