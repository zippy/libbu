/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/client.h"
#include "bu/socket.h"
#include <stdlib.h>
#include <errno.h>
#include "bu/exceptions.h"
#include "bu/protocol.h"
#include "bu/clientlink.h"
#include "bu/clientlinkfactory.h"

/** Read buffer size. */
#define RBS		(1024*2)

Bu::Client::Client( Bu::Socket *pSocket, class Bu::ClientLinkFactory *pfLink ) :
	pSocket( pSocket ),
	pProto( NULL ),
	nRBOffset( 0 ),
	bWantsDisconnect( false ),
	pfLink( pfLink )
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
		try
		{
			nRead = pSocket->read( buf, RBS );

			if( nRead == 0 )
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
		catch( ConnectionException &e )
		{
			pSocket->close();
			bWantsDisconnect = true;
			break;
		}
	}

	if( nTotal == 0 )
	{
		pSocket->close();
		bWantsDisconnect = true;
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
		int nAmnt = (sWriteBuf.getSize()<2048)?(sWriteBuf.getSize()):(2048);
		int nReal = pSocket->write( sWriteBuf.getStr(), nAmnt );
		sWriteBuf.trimFront( nReal );
		//sWriteBuf.clear();
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

void Bu::Client::write( const Bu::FString &sData )
{
	sWriteBuf += sData;
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

void Bu::Client::peek( void *pData, int nBytes, int nOffset )
{
	memcpy( pData, sReadBuf.getStr()+nRBOffset+nOffset, nBytes );
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
	bWantsDisconnect = true;
}

bool Bu::Client::wantsDisconnect()
{
	return bWantsDisconnect;
}

void Bu::Client::close()
{
	pSocket->close();
}

Bu::ClientLink *Bu::Client::getLink()
{
	return pfLink->createLink( this );
}

void Bu::Client::onMessage( const Bu::FString &sMsg )
{
	if( pProto )
		pProto->onMessage( this, sMsg );
}

