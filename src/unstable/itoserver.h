/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_SERVER_H
#define BU_ITO_SERVER_H

#include <stdint.h>

#ifndef WIN32
    #include <sys/select.h>
#endif

#include "bu/string.h"
#include "bu/list.h"
#include "bu/thread.h"
#include "bu/mutex.h"
#include "bu/synchroqueue.h"
#include "bu/hash.h"

#include "bu/clientlink.h"
#include "bu/clientlinkfactory.h"

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
     *@ingroup Threading Serving
     */
    class ItoServer : public Thread
    {
        friend class ItoClient;
        friend class SrvClientLinkFactory;
    public:
        ItoServer();
        virtual ~ItoServer();

#ifdef WIN32
        typedef unsigned int socket_t;
#else
        typedef int socket_t;
#endif

        void addPort( int nPort, int nPoolSize=40 );
        void addPort( const String &sAddr, int nPort, int nPoolSize=40 );

        //void scan();
        void setTimeout( int nTimeoutSec, int nTimeoutUSec=0 );

        void addClient( socket_t nSocket, int nPort );

        virtual void onNewConnection( Client *pClient, int nPort )=0;
        virtual void onClosedConnection( Client *pClient )=0;

    protected:
        virtual void run();

    private:
        class SrvClientLink;
        class ItoClient : public Thread
        {
        friend class Bu::ItoServer::SrvClientLink;
        public:
            ItoClient( ItoServer &rSrv, socket_t nSocket, int nPort,
                    int nTimeoutSec, int nTimeoutUSec );
            virtual ~ItoClient();

            typedef SynchroQueue<Bu::String *> StringQueue;
            StringQueue qMsg;

        protected:
            virtual void run();

        private:
            ItoServer &rSrv;
            Client *pClient;
            fd_set fdActive;
            socket_t iSocket;
            int iPort;
            int nTimeoutSec;
            int nTimeoutUSec;
            Mutex imProto;
        };

        class SrvClientLink : public Bu::ClientLink
        {
        public:
            SrvClientLink( ItoClient *pClient );
            virtual ~SrvClientLink();

            virtual void sendMessage( const Bu::String &sMsg );

        private:
            ItoClient *pClient;
        };

        class SrvClientLinkFactory : public Bu::ClientLinkFactory
        {
        public:
            SrvClientLinkFactory( ItoServer &rSrv );
            virtual ~SrvClientLinkFactory();

            virtual Bu::ClientLink *createLink( Bu::Client *pClient );

        private:
            ItoServer &rSrv;
        };

        int nTimeoutSec;
        int nTimeoutUSec;
        fd_set fdActive;
        typedef Hash<socket_t,TcpServerSocket *> ServerHash;
        ServerHash hServers;
        typedef Hash<socket_t,ItoClient *> ClientHash;
        typedef SynchroQueue<ItoClient *> ClientQueue;
        ClientHash hClients;
        ClientQueue qClientCleanup;
        Mutex imClients;

        void clientCleanup( socket_t iSocket );
    };
}

#endif
