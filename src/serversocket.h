#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <stdint.h>
#include "fstring.h"
#include "socket.h"

namespace Bu
{
	/**
	 *
	 */
	class ServerSocket
	{
	public:
		ServerSocket( int nPort, int nPoolSize=40 );
		ServerSocket( const FString &sAddr, int nPort, int nPoolSize=40 );
		virtual ~ServerSocket();

		int accept( int nTimeoutSec, int nTimeoutUSec );

	private:
		void startServer( struct sockaddr_in &name, int nPoolSize );

		fd_set fdActive;
		int nServer;
		int nPort;
	};
}

#endif
