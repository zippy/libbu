#include "bu/server.h"
#include "bu/client.h"
#include "bu/protocol.h"

class TickProtocol : public Bu::Protocol
{
public:
	TickProtocol()
	{
	}

	virtual ~TickProtocol()
	{
	}

	virtual void onTick( Bu::Client *pClient )
	{
		printf("tick!\n");
		pClient->write("tick!\n");
	}
};

class TickServer : public Bu::Server
{
public:
	TickServer()
	{
	}

	virtual ~TickServer()
	{
	}

	virtual void onNewConnection( Bu::Client *pClient, int )
	{
		pClient->setProtocol( new TickProtocol() );
	}

	virtual void onClosedConnection( Bu::Client *pClient )
	{
		delete pClient->getProtocol();
	}
};

int main( int , char *[] )
{
	TickServer ts;

	ts.setTimeout( 1, 0 );
	ts.setAutoTick();
	ts.addPort( 5555 );

	for(;;)
	{
		ts.scan();
		sleep( 1 );
	}

	return 0;
}

