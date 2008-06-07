#ifndef BU_MULTI_SERVER_H
#define BU_MULTI_SERVER_H

#include "bu/server.h"
#include "bu/hash.h"

namespace Bu
{
	class Protocol;
	class Client;

	template<class T>
	Protocol *genProtocol()
	{
		return new T;
	}

	class MultiServer : protected Server
	{
	public:
		MultiServer();
		virtual ~MultiServer();

		void addProtocol( Protocol *(*proc)(), int iPort, int nPoolSize=40 );
		void addProtocol( Protocol *(*proc)(), const FString &sAddr, int iPort,
				int nPoolSize=40 );
		
		void scan()
		{
			Server::scan();
		}

		void setTimeout( int nTimeoutSec, int nTimeoutUSec=0 )
		{
			Server::setTimeout( nTimeoutSec, nTimeoutUSec );
		}

		virtual void onNewConnection( Client *pClient, int nPort );
		virtual void onClosedConnection( Client *pClient );

	private:
		Bu::Hash<int, Protocol *(*)()> hProtos;
	};
}

#endif