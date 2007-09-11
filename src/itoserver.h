#ifndef BU_ITO_SERVER_H
#define BU_ITO_SERVER_H

#include <stdint.h>

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/ito.h"
#include "bu/itomutex.h"
#include "bu/itoqueue.h"
#include "bu/set.h"

namespace Bu
{
	class ServerSocket;
	class Socket;
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
	 */
	class ItoServer : public Ito
	{
		friend class ItoClient;
	public:
		ItoServer();
		virtual ~ItoServer();

		void addPort( int nPort, int nPoolSize=40 );
		void addPort( const FString &sAddr, int nPort, int nPoolSize=40 );

		//void scan();
		void setTimeout( int nTimeoutSec, int nTimeoutUSec=0 );

		void addClient( int nSocket, int nPort );

		virtual void onNewConnection( Client *pClient, int nPort )=0;
		virtual void onClosedConnection( Client *pClient )=0;

		virtual void *run();

	private:
		class ItoClient : public Ito
		{
		public:
			ItoClient( ItoServer &rSrv, int nSocket, int nPort,
					int nTimeoutSec, int nTimeoutUSec );
			virtual ~ItoClient();

			virtual void *run();

		private:
			ItoServer &rSrv;
			Client *pClient;
			fd_set fdActive;
			int iSocket;
			int iPort;
			int nTimeoutSec;
			int nTimeoutUSec;
		};

		int nTimeoutSec;
		int nTimeoutUSec;
		fd_set fdActive;
		typedef Hash<int,ServerSocket *> ServerHash;
		ServerHash hServers;
		typedef Hash<int,ItoClient *> ClientHash;
		typedef ItoQueue<ItoClient *> ClientQueue;
		ClientHash hClients;
		ClientQueue qClientCleanup;
		ItoMutex imClients;

		void clientCleanup( int iSocket );
	};
}

#endif
