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
	pProto( NULL ),
	nRBOffset( 0 )
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

void Bu::Client::processOutput()
{
	if( sWriteBuf.getSize() > 0 )
	{
		pSocket->write( sWriteBuf.getStr(), sWriteBuf.getSize() );
		sWriteBuf.clear();
	}
}

void Bu::Client::setProtocol( Protocol *pProto )
{
	this->pProto = pProto;
	this->pProto->onNewConnection( this );
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

Bu::FString &Bu::Client::getOutput()
{
	return sWriteBuf;
}

bool Bu::Client::isOpen()
{
	if( !pSocket ) return false;
	return pSocket->isOpen();
}

void Bu::Client::write( const void *pData, int nBytes )
{
	sWriteBuf.append( (const char *)pData, nBytes );
}

void Bu::Client::write( int8_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( int16_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( int32_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( int64_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( uint8_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( uint16_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( uint32_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::write( uint64_t nData )
{
	sWriteBuf.append( (const char *)&nData, sizeof(nData) );
}

void Bu::Client::read( void *pData, int nBytes )
{
	memcpy( pData, sReadBuf.getStr()+nRBOffset, nBytes );
	nRBOffset += nBytes;
	if( sReadBuf.getSize()-nRBOffset == 0 )
	{
		sReadBuf.clear();
		nRBOffset = 0;
	}
	// This is an experimental threshold, maybe I'll make this configurable
	// later on.
	else if(
		(sReadBuf.getSize() >= 1024 && nRBOffset >= sReadBuf.getSize()/2) ||
		(nRBOffset >= sReadBuf.getSize()/4)
		)
	{
		sReadBuf.trimFront( nRBOffset );
		nRBOffset = 0;
	}
}

void Bu::Client::peek( void *pData, int nBytes )
{
	memcpy( pData, sReadBuf.getStr()+nRBOffset, nBytes );
}

void Bu::Client::seek( int nBytes )
{
	nRBOffset += nBytes;
	if( sReadBuf.getSize()-nRBOffset == 0 )
	{
		sReadBuf.clear();
		nRBOffset = 0;
	}
	// This is an experimental threshold, maybe I'll make this configurable
	// later on.
	else if(
		(sReadBuf.getSize() >= 1024 && nRBOffset >= sReadBuf.getSize()/2) ||
		(nRBOffset >= sReadBuf.getSize()/4)
		)
	{
		sReadBuf.trimFront( nRBOffset );
		nRBOffset = 0;
	}
}

long Bu::Client::getInputSize()
{
	return sReadBuf.getSize()-nRBOffset;
}

const Bu::Socket *Bu::Client::getSocket() const
{
	return pSocket;
}

void Bu::Client::disconnect()
{
}

