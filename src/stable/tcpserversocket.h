/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TCP_SERVER_SOCKET_H
#define BU_TCP_SERVER_SOCKET_H

#include <stdint.h>
#include "bu/string.h"
#include "bu/exceptionbase.h"

#ifdef WIN32
 #include <Winsock2.h>
#else
 #include <sys/select.h>
#endif

namespace Bu
{
    subExceptionDecl( TcpServerSocketException );

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
     *@ingroup Serving
     */
    class TcpServerSocket
    {
    public:
#ifdef WIN32
        typedef unsigned int socket_t;
#else
        typedef int socket_t;
#endif
        TcpServerSocket( int nPort, int nPoolSize=40 );
        TcpServerSocket( const String &sAddr, int nPort, int nPoolSize=40 );
        TcpServerSocket( socket_t nSocket, bool bInit, int nPoolSize=40 );
        TcpServerSocket( const TcpServerSocket &rSrc );
        virtual ~TcpServerSocket();

        int accept( int nTimeoutSec=0, int nTimeoutUSec=0 );
        int getSocket();
        int getPort();

    private:
        void startServer( struct sockaddr_in &name, int nPoolSize );
        void initServer( struct sockaddr_in &name, int nPoolSize );

        fd_set fdActive;
        socket_t nServer;
        int nPort;
    };
}

#endif
