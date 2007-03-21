#ifndef SERIALIZER_CONNECTION_H
#define SERIALIZER_CONNECTION_H

#include <stdint.h>

#include "serializer.h"
#include "connection.h"

/**
 *
 */
class SerializerConnection : public Serializer
{
public:
	SerializerConnection( Connection *pCon, bool bIO, int nTimeSec, int nTimeUSec );
	virtual ~SerializerConnection();

private:
	Connection *pCon;
	int nTimeSec;
	int nTimeUSec;
};

#endif
