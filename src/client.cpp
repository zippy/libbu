#include "bu/client.h"
#include "bu/socket.h"
#include <stdlib.h>
#include <errno.h>
#include "bu/exceptions.h"
#include "bu/protocol.h"

/** Read buffer size. */
#define RBS		(1024*2)

Bu::Client::Client( Bu::Socket *pSocket ) :
	pSocket( pSocket ),
	pProto( NULL )
{
}

Bu::Client::~Client()
{
}

void Bu::Client::processInput()
{
	char buf[RBS];
	size_t nRead, nTotal=0;

	for(;;)
	{
		nRead = pSocket->read( buf, nRead );
		if( nRead < 0  )
		{
			throw Bu::ConnectionException(
				excodeReadError,
				"Read error: %s",
				strerror( errno )
				);
		}
		else if( nRead == 0 )
		{
			break;
		}
		else
		{
			nTotal += nRead;
			sReadBuf.append( buf, nRead );
			if( !pSocket->canRead() )
				break;
		}
	}

	if( nTotal == 0 )
	{
		pSocket->close();
	}

	if( pProto && nTotal )
	{
		pProto->onNewData( this );
	}
}

void Bu::Client::setProtocol( Protocol *pProto )
{
	this->pProto = pProto;
}

Bu::Protocol *Bu::Client::getProtocol()
{
	return pProto;
}

void Bu::Client::clearProtocol()
{
	pProto = NULL;
}

Bu::FString &Bu::Client::getInput()
{
	return sReadBuf;
}

