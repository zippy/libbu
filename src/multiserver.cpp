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

