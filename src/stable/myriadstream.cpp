/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/myriadstream.h"

#include <string.h>

// #define MYRIAD_STREAM_DEBUG  1

#ifdef MYRIAD_STREAM_DEBUG
#include "bu/sio.h"

using Bu::sio;
using Bu::Fmt;
#endif

Bu::MyriadStream::MyriadStream( Bu::Myriad &rMyriad,
        Bu::Myriad::Stream *pStream ) :
    rMyriad( rMyriad ),
    pStream( pStream ),
    pCurBlock( NULL ),
    iPos( 0 )
{
#ifdef MYRIAD_STREAM_DEBUG
    sio << "MyriadStream: " << __LINE__ << ": Created, iId=" << pStream->iId << ", iSize="
        << pStream->iSize << sio.nl;
#endif
    //pCurBlock = rMyriad.newBlock();
    //rMyriad.getBlock( uStream, pCurBlock );
    //uSize = pCurBlock->uBytesUsed;
}

Bu::MyriadStream::~MyriadStream()
{
    if( pCurBlock )
        rMyriad.releaseBlock( pCurBlock );
    //rMyriad.updateStreamSize( uStream, uSize );
    //rMyriad.deleteBlock( pCurBlock );
}

void Bu::MyriadStream::close()
{
}

Bu::size Bu::MyriadStream::read( void *pBuf, Bu::size nBytes )
{
#ifdef MYRIAD_STREAM_DEBUG
    sio << "MyriadStream: read: " << __LINE__ << ": Started, asked to read " << nBytes << "b."
        << sio.nl;
#endif
    if( nBytes > (Bu::size)pStream->iSize-iPos )
        nBytes = pStream->iSize-iPos;
    if( nBytes <= 0 )
        return 0;
    int iLeft = nBytes;
#ifdef MYRIAD_STREAM_DEBUG
    sio << "MyriadStream: read: " << __LINE__ << ": Started, going to read " << nBytes << "b."
        << sio.nl;
#endif
    if( pCurBlock == NULL )
    {
#ifdef MYRIAD_STREAM_DEBUG
        sio << "MyriadStream: read: " << __LINE__ << ": No block loaded, loading initial block."
            << sio.nl;
#endif
        pCurBlock = rMyriad.getBlock(
            pStream->aBlocks[iPos/rMyriad.iBlockSize]
            );
    }
    while( iLeft > 0 )
    {
        int iCurBlock = pStream->aBlocks[iPos/rMyriad.iBlockSize];
        if( pCurBlock->iBlockIndex != iCurBlock )
        {
#ifdef MYRIAD_STREAM_DEBUG
            sio << "MyriadStream: read: " << __LINE__ << ": Loading new block " << iCurBlock << "."
                << sio.nl;
#endif
            rMyriad.releaseBlock( pCurBlock );
            pCurBlock = rMyriad.getBlock( iCurBlock );
        }

        int iAmnt = Bu::buMin(
            Bu::buMin(
                rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
                iLeft
                ),
            pStream->iSize-iPos
            );
#ifdef MYRIAD_STREAM_DEBUG
        sio << "MyriadStream: read: " << __LINE__ << ": Copying out bytes: "
            << iPos << "(" << (iPos%rMyriad.iBlockSize) << ")+"
            << iAmnt
            << ", " << iLeft << "b left." << sio.nl;
#endif
        memcpy(
            pBuf,
            pCurBlock->pData+(iPos%rMyriad.iBlockSize),
            iAmnt
            );
        iPos += iAmnt;
        pBuf = &((char *)pBuf)[iAmnt];
        iLeft -= iAmnt;
    }
    return nBytes;
}

Bu::size Bu::MyriadStream::write( const void *pBuf, Bu::size nBytes )
{
    if( nBytes <= 0 )
        return 0;

#ifdef MYRIAD_STREAM_DEBUG
    sio << "MyriadStream: write: " << __LINE__ << ": Started, asked to write " << nBytes << "b."
        << sio.nl;
#endif
    if( nBytes <= 0 )
        return 0;
    int iLeft = nBytes;
    /*
    if( pCurBlock == NULL )
    {
#ifdef MYRIAD_STREAM_DEBUG
        sio << "MyriadStream: write: No block loaded, loading initial block."
            << sio.nl;
#endif
        pCurBlock = rMyriad.getBlock(
            pStream->aBlocks[iPos/rMyriad.iBlockSize]
            );
    }*/
    
    while( iLeft > 0 )
    {
        int iCurBlock;
        if( iPos/rMyriad.iBlockSize < pStream->aBlocks.getSize() )
        {
            iCurBlock = pStream->aBlocks[iPos/rMyriad.iBlockSize];
        }
        else
        {
            iCurBlock = rMyriad.streamAddBlock( pStream );
#ifdef MYRIAD_STREAM_DEBUG
            sio << "MyriadStream: write: " << __LINE__ << ": New block allocated and appended: "
                << iCurBlock << "." << sio.nl;

#endif
        }
        if( !pCurBlock || pCurBlock->iBlockIndex != iCurBlock )
        {
#ifdef MYRIAD_STREAM_DEBUG
            sio << "MyriadStream: write: " << __LINE__ << ": Loading new block " << iCurBlock << "."
                << sio.nl;
#endif
            rMyriad.releaseBlock( pCurBlock );
            pCurBlock = rMyriad.getBlock( iCurBlock );
        }
        pCurBlock->bChanged = true;
        
        // There are two main writing modes when it comes down to it.
        // Overwrite mode and append mode.  Append is what pretty much always
        // happens when creating a new stream.
        if( iPos < pStream->iSize )
        {
            int iAmnt = Bu::buMin(
                Bu::buMin(
                    rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
                    iLeft
                    ),
                pStream->iSize-iPos
                );
#ifdef MYRIAD_STREAM_DEBUG
            sio << "MyriadStream: write (ovr): " << __LINE__ << ": Copying in bytes: "
                << (iPos%rMyriad.iBlockSize) << "+"
                << iAmnt
                << ", " << iLeft << "b left." << sio.nl;
#endif
            memcpy(
                pCurBlock->pData+(iPos%rMyriad.iBlockSize),
                pBuf,
                iAmnt
                );
            iPos += iAmnt;
            pBuf = &((char *)pBuf)[iAmnt];
            iLeft -= iAmnt;
        }
        else
        {
            int iAmnt = Bu::buMin(
                rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
                iLeft
                );
#ifdef MYRIAD_STREAM_DEBUG
            sio << "MyriadStream: write (app): " << __LINE__ << ": Copying in bytes: "
                << (iPos%rMyriad.iBlockSize) << "+"
                << iAmnt
                << ", " << iLeft << "b left." << sio.nl;
#endif
            memcpy(
                pCurBlock->pData+(iPos%rMyriad.iBlockSize),
                pBuf,
                iAmnt
                );
            iPos += iAmnt;
            pStream->iSize += iAmnt;
            rMyriad.headerChanged();
            pBuf = &((char *)pBuf)[iAmnt];
            iLeft -= iAmnt;
        }
    }

    return nBytes;
}

Bu::size Bu::MyriadStream::tell()
{
    return iPos;
}

void Bu::MyriadStream::seek( Bu::size offset )
{
    iPos += offset;
}

void Bu::MyriadStream::setPos( Bu::size pos )
{
    iPos = pos;
}

void Bu::MyriadStream::setPosEnd( Bu::size pos )
{
    iPos = pStream->iSize-pos;
}

bool Bu::MyriadStream::isEos()
{
    return iPos >= pStream->iSize;
}

bool Bu::MyriadStream::isOpen()
{
    return true;
}

void Bu::MyriadStream::flush()
{
}

bool Bu::MyriadStream::canRead()
{
    return true;
}

bool Bu::MyriadStream::canWrite()
{
    return true;
}

bool Bu::MyriadStream::isReadable()
{
    return true;
}

bool Bu::MyriadStream::isWritable()
{
    return true;
}

bool Bu::MyriadStream::isSeekable()
{
    return true;
}

bool Bu::MyriadStream::isBlocking()
{
    return true;
}

void Bu::MyriadStream::setBlocking( bool /*bBlocking*/ )
{
}

void Bu::MyriadStream::setSize( Bu::size iSize )
{
    if( iSize < 0 )
        iSize = 0;
    rMyriad.setStreamSize( pStream, iSize );
    if( iPos > iSize )
        iPos = iSize;
}

Bu::size Bu::MyriadStream::getSize() const
{
    return pStream->iSize;
}

Bu::size Bu::MyriadStream::getBlockSize() const
{
    return rMyriad.getBlockSize();
}

Bu::String Bu::MyriadStream::getLocation() const
{
    return Bu::String("%1").arg( pStream->iId );
}

