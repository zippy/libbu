#include "connectionmanager.h"
#include "programlink.h"
#include "linkedlist.h"
#include "protocol.h"

class StressProtocol : public Protocol
{
public:
	bool onNewData()
	{
		switch( getConnection()->getInput()[0] )
		{
			case 'd':
				throw "Hello";
				break;

			case 'w':
				getConnection()->appendOutput("Hello");
				break;
		};

		return true;
	}

	bool onNewConnection()
	{
		return true;
	}
};

class StressMonitor : public ConnectionMonitor, public ProgramLink
{
public:
	bool init()
	{
		return true;
	}

	bool deInit()
	{
		return true;
	}

	bool timeSlice()
	{
	}

	bool onNewConnection( Connection *pCon, int nPort )
	{
		StressProtocol *sp = new StressProtocol();
		pCon->setProtocol( sp );

		printf("    sys: New connection: socket(%d), port(%d)\n",
				pCon->getSocket(), nPort );

		return true;
	}

	bool onClosedConnection( Connection *pCon )
	{
		printf("    sys: Closed connection: socket(%d)\n",
				pCon->getSocket() );

		return true;
	}

	LinkMessage *processIRM( LinkMessage *pMsg )
	{
		return NULL;
	}
};

int main()
{
	printf("Starting server...\n");

	ConnectionManager srv;
	StressMonitor telnet;

	srv.setConnectionMonitor( &telnet );

	srv.startServer( 4001 );

	for(;;)
	{
		srv.scanConnections( 5000, false );
	}
	
	return 0;
}
