/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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

	virtual void gotLine( Bu::String &sLine )
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

	virtual void onNewConnection( Bu::Client *pClient, int )
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

int main()
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

