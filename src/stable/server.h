/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SERVER_H
#define BU_SERVER_H

#include <stdint.h>

#ifndef WIN32
    #include <sys/select.h>
#endif

#include "bu/string.h"
#include "bu/list.h"

#include "bu/clientlink.h"
#include "bu/clientlinkfactory.h"
#include "bu/hash.h"

#include "bu/config.h"

namespace Bu
{
    class TcpServerSocket;
    class TcpSocket;
    class Client;

    /**
     * Core of a network server.  This class is distinct from a ServerSocket in
     * that a ServerSocket is one listening socket, nothing more.  Socket will
     * manage a pool of both ServerSockets and connected Sockets along with
     * their protocols and buffers.
     *
     * To start serving on a new port, use the addPort functions.  Each call to
     * addPort creates a new ServerSocket, starts it listening, and adds it to
     * the server pool.
     *
     * All of the real work is done by scan, which will wait for up
     * to the timeout set by setTimeout before returning if there is no data
     * pending.  scan should probably be called in some sort of tight
     * loop, possibly in it's own thread, or in the main control loop.
     *
     * In order to use a Server you must subclass it and implement the pure
     * virtual functions.  These allow you to receive notification of events
     * happening within the server itself, and actually makes it useful.
     *@ingroup Serving
     */
    class Server
    {
    public:
        Server();
        virtual ~Server();

#ifdef WIN32
        typedef unsigned int socket_t;
#else
        typedef int socket_t;
#endif

        void addPort( int nPort, int nPoolSize=40 );
        void addPort( const String &sAddr, int nPort, int nPoolSize=40 );

        virtual void scan();
        void setTimeout( int nTimeoutSec, int nTimeoutUSec=0 );

        void addClient( socket_t nSocket, int nPort );

        void setAutoTick( bool bEnable=true );
        void tick();

        virtual void onNewConnection( Client *pClient, int nPort )=0;
        virtual void onClosedConnection( Client *pClient )=0;

        void shutdown();

    private:
        void closeClient( socket_t iSocket );
        class SrvClientLink : public Bu::ClientLink
        {
        public:
            SrvClientLink( Bu::Client *pClient );
            virtual ~SrvClientLink();

            virtual void sendMessage( const Bu::String &sMsg );

        private:
            Bu::Client *pClient;
        };

        class SrvClientLinkFactory : public Bu::ClientLinkFactory
        {
        public:
            SrvClientLinkFactory();
            virtual ~SrvClientLinkFactory();

            virtual Bu::ClientLink *createLink( Bu::Client *pClient );
        };

        int nTimeoutSec;
        int nTimeoutUSec;
        fd_set fdActive;
        typedef Hash<socket_t,TcpServerSocket *> SrvHash;
        SrvHash hServers;
        typedef Hash<socket_t,Client *> ClientHash;
        ClientHash hClients;
        bool bAutoTick;
    };
}

#endif
