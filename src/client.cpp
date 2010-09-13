/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/client.h"
#include "bu/socket.h"
#include <stdlib.h>
#include <errno.h>
#include "bu/protocol.h"
#include "bu/clientlink.h"
#include "bu/clientlinkfactory.h"

/** Read buffer size. */
#define RBS		(1024*2)

Bu::Client::Client( Bu::Socket *pSocket, class Bu::ClientLinkFactory *pfLink ) :
	pTopStream( pSocket ),
	pSocket( pSocket ),
	pProto( NULL ),
	bWantsDisconnect( false ),
	pfLink( pfLink )
{
	lFilts.prepend( pSocket );
}

Bu::Client::~Client()
{
	for( FilterList::iterator i = lFilts.begin(); i; i++ )
	{
		delete *i;
	}
	pTopStream = pSocket = NULL;
	delete pfLink;
}

void Bu::Client::processInput()
{
	char buf[RBS];
	size_t nRead, nTotal=0;

	for(;;)
	{
		try
		{
			nRead = pTopStream->read( buf, RBS );

			if( nRead == 0 )
			{
				break;
			}
			else
			{
				nTotal += nRead;
				qbRead.write( buf, nRead );
				if( !pTopStream->canRead() )
					break;
			}
		}
		catch( Bu::SocketException &e )
		{
			pTopStream->close();
			bWantsDisconnect = true;
			break;
		}
	}

	if( nTotal == 0 )
	{
		pTopStream->close();
		bWantsDisconnect = true;
	}

	if( pProto && nTotal )
	{
		pProto->onNewData( this );
	}
}

void Bu::Client::processOutput()
{
	if( qbWrite.getSize() > 0 )
	{
		int nAmnt = RBS;
		char *buf = new char[nAmnt];
		nAmnt = qbWrite.peek( buf, nAmnt );
		int nReal = pTopStream->write( buf, nAmnt );
		qbWrite.seek( nReal );
		pTopStream->flush();
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
/*
Bu::FString &Bu::Client::getInput()
{
	return sReadBuf;
}

Bu::FString &Bu::Client::getOutput()
{
	return sWriteBuf;
}
*/

bool Bu::Client::isOpen()
{
	if( !pTopStream ) return false;
	return pTopStream->isOpen();
}

size_t Bu::Client::write( const Bu::FString &sData )
{
	return qbWrite.write( sData.getStr(), sData.getSize() );
}

size_t Bu::Client::write( const void *pData, size_t nBytes )
{
	return qbWrite.write( pData, nBytes );
}

size_t Bu::Client::write( int8_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( int16_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( int32_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( int64_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( uint8_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( uint16_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( uint32_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::write( uint64_t nData )
{
	return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

size_t Bu::Client::read( void *pData, size_t nBytes )
{
	return qbRead.read( pData, nBytes );
}

size_t Bu::Client::peek( void *pData, int nBytes, int nOffset )
{
	return qbRead.peek( pData, nBytes, nOffset );
}

long Bu::Client::getInputSize()
{
	return qbRead.getSize();
}

long Bu::Client::getOutputSize()
{
	return qbWrite.getSize();
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
	pTopStream->close();
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

void Bu::Client::tick()
{
	if( pProto )
		pProto->onTick( this );
}

long Bu::Client::tell()
{
}

void Bu::Client::seek( long offset )
{
	return qbRead.seek( offset );
}

void Bu::Client::setPos( long pos )
{
}

void Bu::Client::setPosEnd( long pos )
{
}

bool Bu::Client::isEos()
{
	return true;
}

void Bu::Client::flush()
{
}

bool Bu::Client::canRead()
{
	return qbRead.getSize() > 0;
}

bool Bu::Client::canWrite()
{
	return true;
}

bool Bu::Client::isReadable()
{
	return true;
}

bool Bu::Client::isWritable()
{
	return true;
}

bool Bu::Client::isSeekable()
{
	return false;
}

bool Bu::Client::isBlocking()
{
	return false;
}

void Bu::Client::setBlocking( bool bBlocking )
{
}

void Bu::Client::setSize( long iSize )
{
}

