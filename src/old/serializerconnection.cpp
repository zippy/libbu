#include "serializerconnection.h"

SerializerConnection::SerializerConnection(
		Connection *pCon, bool bIO, int nTimeSec, int nTimeUSec ) :
	Serializer( bIO ),
	pCon( pCon ),
	nTimeSec( nTimeSec ),
	nTimeUSec( nTimeUSec )
{
}

SerializerConnection::~SerializerConnection()
{
}

