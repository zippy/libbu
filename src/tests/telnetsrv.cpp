#include "bu/server.h"
#include "bu/protocoltelnet.h"
#include "bu/client.h"

class MyTelnet : public Bu::ProtocolTelnet
{
public:
	MyTelnet()
	{
	}

	virtual ~MyTelnet()
	{
	}

	virtual void onNewConnection( Bu::Client *pClient )
	{
		Bu::ProtocolTelnet::onNewConnection( pClient );

		//oNAWS.remoteSet();
		oEcho.localSet();
		oSuppressGA.remoteSet( true );
		oSuppressGA.localSet( true );
		setCanonical();
	}

	virtual void onSubNAWS( uint16_t iWidth, uint16_t iHeight )
	{
		printf("New dim = (%dx%d)\n", iWidth, iHeight );
	}

	virtual void gotLine( Bu::FString &sLine )
	{
		printf("Line: \"%s\"\n", sLine.getStr() );
		write("\n\r", 2 );
	}

private:

};

class TelServer : public Bu::Server
{
public:
	TelServer()
	{
	}

	virtual ~TelServer()
	{
	}

	virtual void onNewConnection( Bu::Client *pClient, int iPort )
	{
		printf("New connection.\n");

		pClient->setProtocol( new MyTelnet() );
	}

	virtual void onClosedConnection( Bu::Client *pClient )
	{
		printf("Lost connection.\n");

		delete pClient->getProtocol();
	}

private:

};

int main( int argc, char *argv[] )
{
	TelServer ts;
	
	ts.addPort( 4000 );
	ts.setTimeout( 0, 5000 );

	printf("Initializing server on port: 4000\n");

	for(;;)
	{
		ts.scan();
	}
}

