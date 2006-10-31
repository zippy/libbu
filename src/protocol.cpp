#include "protocol.h"

Protocol::Protocol()
{
	pConnection = NULL;
}

Protocol::~Protocol()
{
}

void Protocol::setConnection( Connection *pNewConnection )
{
	pConnection = pNewConnection;

	onNewConnection();
}

Connection *Protocol::getConnection()
{
	return pConnection;
}
