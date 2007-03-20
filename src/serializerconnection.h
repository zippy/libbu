#ifndef SERIALIZER_CONNECTION_H
#define SERIALIZER_CONNECTION_H

#include <stdint.h>

#include "serializer.h"

/**
 *
 */
class SerializerConnection : public Serializer
{
public:
	SerializerConnection( Connection *pCon, int nTimeSec, int nTimeUSec );
	virtual ~SerializerConnection();

private:
	Connection *pCon;
	int nTimeSec;
	int nTimeUSec;
};

#endif
