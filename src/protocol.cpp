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
}

Connection *Protocol::getConnection()
{
	return pConnection;
}
