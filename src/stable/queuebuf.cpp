/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/queuebuf.h"

#include "bu/sio.h"
using Bu::sio;

Bu::QueueBuf::QueueBuf( int iBlockSize /*=256*/ ) :
    iBlockSize( iBlockSize ),
    iReadOffset( 0 ),
    iWriteOffset( 0 ),
    iTotalSize( 0 )
{
}

Bu::QueueBuf::~QueueBuf()
{
    for( BlockList::iterator i = lBlocks.begin(); i; i++ )
        delete[] *i;
}

void Bu::QueueBuf::close()
{
    for( BlockList::iterator i = lBlocks.begin(); i; i++ )
        delete[] *i;
    lBlocks.clear();
    iReadOffset = iWriteOffset = iTotalSize = 0;
}

Bu::size Bu::QueueBuf::read( void *pRawBuf, Bu::size nBytes )
{
    if( nBytes <= 0 )
        return 0;

    if( lBlocks.isEmpty() )
        return 0;

    Bu::size iLeft = nBytes;
    char *pBuf = (char *)pRawBuf;

    while( iLeft > 0 && iTotalSize > 0 )
    {
        if( iReadOffset == iBlockSize )
        {
            removeBlock();
            if( lBlocks.isEmpty() )
            {
                return nBytes-iLeft;
            }
            iReadOffset = 0;
        }
        char *pBlock = lBlocks.first();
        Bu::size iCopy = iBlockSize-iReadOffset;
        if( iLeft < iCopy )
            iCopy = iLeft;
        if( iTotalSize < iCopy )
            iCopy = iTotalSize;
        memcpy( pBuf, pBlock+iReadOffset, iCopy );
        iReadOffset += iCopy;
        iLeft -= iCopy;
        pBuf += iCopy;
        iTotalSize -= iCopy;
//      sio << "Read " << iCopy << " bytes, new size: " << iTotalSize << sio.nl;
    }

    return nBytes - iLeft;
}

Bu::size Bu::QueueBuf::peek( void *pBuf, Bu::size nBytes )
{
    return peek( pBuf, nBytes, 0 );
}

Bu::size Bu::QueueBuf::peek( void *pRawBuf, Bu::size nBytes, Bu::size nSkip )
{
    if( nBytes <= 0 )
        return 0;

    if( lBlocks.isEmpty() )
        return 0;

    Bu::size iLeft = nBytes;
    char *pBuf = (char *)pRawBuf;

    int iTmpReadOffset = iReadOffset + nSkip;
    Bu::size iTmpRemSize = iTotalSize;
    BlockList::iterator iBlock = lBlocks.begin();
    while( iTmpReadOffset > iBlockSize )
    {
        iTmpReadOffset -= iBlockSize;
        iBlock++;
    }
    while( iLeft > 0 && iTmpRemSize > 0 )
    {
        if( iTmpReadOffset == iBlockSize )
        {
            iBlock++;
            if( iBlock == lBlocks.end() )
            {
                return nBytes-iLeft;
            }
            iTmpReadOffset = 0;
        }
        char *pBlock = *iBlock;
        Bu::size iCopy = iBlockSize-iTmpReadOffset;
        if( iLeft < iCopy )
            iCopy = iLeft;
        if( iTmpRemSize < iCopy )
            iCopy = iTmpRemSize;
        memcpy( pBuf, pBlock+iTmpReadOffset, iCopy );
        iTmpReadOffset += iCopy;
        iLeft -= iCopy;
        pBuf += iCopy;
        iTmpRemSize -= iCopy;
//      sio << "Read (peek) " << iCopy << " bytes, new temp size: "
//          << iTmpRemSize << sio.nl;
    }

    return nBytes - iLeft;
}

Bu::size Bu::QueueBuf::write( const void *pRawBuf, Bu::size nBytes )
{
    if( nBytes <= 0 )
        return 0;

    if( lBlocks.isEmpty() )
    {
        addBlock();
        iWriteOffset = 0;
    }
    Bu::size iLeft = nBytes;
    const char *pBuf = (const char *)pRawBuf;

    while( iLeft > 0 )
    {
        if( iWriteOffset == iBlockSize )
        {
            addBlock();
            iWriteOffset = 0;
        }
        char *pBlock = lBlocks.last();
        Bu::size iCopy = iBlockSize-iWriteOffset;
        if( iLeft < iCopy )
            iCopy = iLeft;
        memcpy( pBlock+iWriteOffset, pBuf, iCopy );
        iWriteOffset += iCopy;
        iLeft -= iCopy;
        pBuf += iCopy;
        iTotalSize += iCopy;
//      sio << "Wrote " << iCopy << " bytes, new size: " << iTotalSize
//          << sio.nl;
    }

    return nBytes;
}

Bu::size Bu::QueueBuf::tell()
{
    return -1;
}

void Bu::QueueBuf::seek( Bu::size iAmnt )
{
    if( iAmnt <= 0 )
        return;

    if( (Bu::size)iAmnt >= iTotalSize )
    {
//      sio << "seek: clear all data (" << iAmnt << ">=" << iTotalSize
//          << ")." << sio.nl;
        close();
        return;
    }

    iReadOffset += iAmnt;
    iTotalSize -= iAmnt;
    while( iReadOffset >= iBlockSize )
    {
        removeBlock();
        iReadOffset -= iBlockSize;
//      sio << "seek: removal step (" << iReadOffset << ")" << sio.nl;
    }
}

void Bu::QueueBuf::setPos( Bu::size )
{
}

void Bu::QueueBuf::setPosEnd( Bu::size )
{
}

bool Bu::QueueBuf::isEos()
{
    return iTotalSize == 0;
}

bool Bu::QueueBuf::isOpen()
{
    return true;
}

void Bu::QueueBuf::flush()
{
}

bool Bu::QueueBuf::canRead()
{
    return iTotalSize > 0;
}

bool Bu::QueueBuf::canWrite()
{
    return true;
}

bool Bu::QueueBuf::isReadable()
{
    return true;
}

bool Bu::QueueBuf::isWritable()
{
    return true;
}

bool Bu::QueueBuf::isSeekable()
{
    return false;
}

bool Bu::QueueBuf::isBlocking()
{
    return false;
}

void Bu::QueueBuf::setBlocking( bool )
{
}

void Bu::QueueBuf::setSize( Bu::size )
{
}

Bu::size Bu::QueueBuf::getSize() const
{
    return iTotalSize;
}

Bu::size Bu::QueueBuf::getBlockSize() const
{
    return iBlockSize;
}

Bu::String Bu::QueueBuf::getLocation() const
{
    return "";
}

void Bu::QueueBuf::addBlock()
{
    lBlocks.append( new char[iBlockSize] );
//  sio << "Added new block." << sio.nl;
}

void Bu::QueueBuf::removeBlock()
{
    delete[] lBlocks.first();
    lBlocks.erase( lBlocks.begin() );
//  sio << "Removed block." << sio.nl;
}

