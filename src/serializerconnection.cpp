#include "serializerconnection.h"

SerializerConnection::SerializerConnection(
		Connection *pCon, int nTimeSec, int nTimeUSec ) :
	pCon( pCon ),
	nTimeSec( nTimeSec ),
	nTimeUSec( nTimeUSec )
{
}

SerializerConnection::~SerializerConnection()
{
}

