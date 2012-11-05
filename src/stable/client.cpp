/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/client.h"
#include "bu/tcpsocket.h"
#include <stdlib.h>
#include <errno.h>
#include "bu/protocol.h"
#include "bu/clientlink.h"
#include "bu/clientlinkfactory.h"

/** Read buffer size. */
#define RBS     (2000) // 1500 is the nominal MTU for ethernet, it's a good guess

Bu::Client::Client( Bu::TcpSocket *pSocket,
        class Bu::ClientLinkFactory *pfLink ) :
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
    Bu::size nRead, nTotal=0;

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
        catch( Bu::TcpSocketException &e )
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
    char buf[RBS];
    if( qbWrite.getSize() > 0 )
    {
        int nAmnt = RBS;
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
Bu::String &Bu::Client::getInput()
{
    return sReadBuf;
}

Bu::String &Bu::Client::getOutput()
{
    return sWriteBuf;
}
*/

bool Bu::Client::isOpen()
{
    if( !pTopStream ) return false;
    return pTopStream->isOpen();
}

Bu::size Bu::Client::write( const Bu::String &sData )
{
    return qbWrite.write( sData.getStr(), sData.getSize() );
}

Bu::size Bu::Client::write( const void *pData, Bu::size nBytes )
{
    return qbWrite.write( pData, nBytes );
}

Bu::size Bu::Client::write( int8_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( int16_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( int32_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( int64_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( uint8_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( uint16_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( uint32_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::write( uint64_t nData )
{
    return qbWrite.write( (const char *)&nData, sizeof(nData) );
}

Bu::size Bu::Client::read( void *pData, Bu::size nBytes )
{
    return qbRead.read( pData, nBytes );
}

Bu::size Bu::Client::peek( void *pData, int nBytes, int nOffset )
{
    return qbRead.peek( pData, nBytes, nOffset );
}

Bu::size Bu::Client::getInputSize()
{
    return qbRead.getSize();
}

Bu::size Bu::Client::getOutputSize()
{
    return qbWrite.getSize();
}

const Bu::TcpSocket *Bu::Client::getSocket() const
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

void Bu::Client::onMessage( const Bu::String &sMsg )
{
    if( pProto )
        pProto->onMessage( this, sMsg );
}

void Bu::Client::tick()
{
    if( pProto )
        pProto->onTick( this );
}

Bu::size Bu::Client::tell()
{
    return 0;
}

void Bu::Client::seek( Bu::size offset )
{
    return qbRead.seek( offset );
}

void Bu::Client::setPos( Bu::size )
{
    throw Bu::ExceptionBase();
}

void Bu::Client::setPosEnd( Bu::size )
{
    throw Bu::ExceptionBase();
}

bool Bu::Client::isEos()
{
    return true;
}

void Bu::Client::flush()
{
    processOutput();
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

void Bu::Client::setBlocking( bool )
{
    throw Bu::ExceptionBase();
}

void Bu::Client::setSize( Bu::size )
{
    throw Bu::ExceptionBase();
}

Bu::size Bu::Client::getSize() const
{
    return 0;
}

Bu::size Bu::Client::getBlockSize() const
{
    return pSocket->getBlockSize();
}

Bu::String Bu::Client::getLocation() const
{
    return pSocket->getLocation();
}

