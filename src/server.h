#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include "bu/serversocket.h"
#include "bu/list.h"
#include "bu/client.h"

namespace Bu
{
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
	 */
	class Server
	{
	public:
		Server();
		virtual ~Server();

		void addPort( int nPort, int nPoolSize=40 );
		void addPort( const FString &sAddr, int nPort, int nPoolSize=40 );

		void scan();
		void setTimeout( int nTimeoutSec, int nTimeoutUSec=0 );

		void addClient( int nSocket );

	private:
		int nTimeoutSec;
		int nTimeoutUSec;
		fd_set fdActive;
		Hash<int,ServerSocket *> hServers;
		Hash<int,Client *> hClients;
	};
}

#endif
