/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "win32_compatibility.h"

#include "bu/multiserver.h"
#include "bu/protocol.h"
#include "bu/client.h"

Bu::MultiServer::MultiServer()
{
}

Bu::MultiServer::~MultiServer()
{
}

void Bu::MultiServer::addProtocol( Bu::Protocol *(*proc)(), int iPort,
	int nPoolSize )
{
	hProtos[iPort] = proc;
	addPort( iPort, nPoolSize );
}

void Bu::MultiServer::addProtocol( Protocol *(*proc)(), const FString &sAddr,
	int iPort, int nPoolSize )
{
	hProtos[iPort] = proc;
	addPort( sAddr, iPort, nPoolSize );
}

void Bu::MultiServer::onNewConnection( Bu::Client *pClient, int nPort )
{
	pClient->setProtocol( hProtos.get( nPort )() );
}

void Bu::MultiServer::onClosedConnection( Bu::Client *pClient )
{
	delete pClient->getProtocol();
}

