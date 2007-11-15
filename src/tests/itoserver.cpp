/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/itoserver.h"
#include "bu/protocol.h"
#include "bu/client.h"

#define BU_TRACE
#include "bu/trace.h"

class ProtocolEcho : public Bu::Protocol
{
public:
	ProtocolEcho()
	{
		TRACE();
	}
	virtual ~ProtocolEcho()
	{
		TRACE();
	}

	virtual void onNewConnection( Bu::Client *pClient )
	{
		TRACE();
		// Huh...
	}

	virtual void onNewData( Bu::Client *pClient )
	{
		TRACE();
		pClient->write( pClient->getInput().getStr(), pClient->getInputSize() );
		pClient->seek( pClient->getInputSize() );
	}
};

class TestServer : public Bu::ItoServer
{
public:
	TestServer()
	{
		TRACE();
	}
	virtual ~TestServer()
	{
		TRACE();
	}

	virtual void onNewConnection( Bu::Client *pClient, int iPort )
	{
		TRACE();
		pClient->setProtocol( new ProtocolEcho() );
	}

	virtual void onClosedConnection( Bu::Client *pClient )
	{
		TRACE();
		delete pClient->getProtocol();
	}
};

int main()
{
	TRACE();
	
	TestServer ts;

	ts.addPort( 5555 );
	ts.start();
	ts.join();
}

