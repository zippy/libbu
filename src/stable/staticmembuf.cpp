/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/staticmembuf.h"

using namespace Bu;

Bu::StaticMemBuf::StaticMemBuf( const void *pData, size iSize ) :
    pData( pData ),
    iSize( iSize ),
    nPos( 0 )
{
}

Bu::StaticMemBuf::~StaticMemBuf()
{
}

void Bu::StaticMemBuf::close()
{
}

size Bu::StaticMemBuf::read( void *pBuf, size nBytes )
{
    if( iSize-nPos < nBytes )
        nBytes = iSize-nPos;

    memcpy( pBuf, ((char *)pData)+nPos, nBytes );
    nPos += nBytes;

    return nBytes;
}
    
size Bu::StaticMemBuf::write( const void *, size )
{
    return -1;
}

size Bu::StaticMemBuf::tell()
{
    return nPos;
}

void Bu::StaticMemBuf::seek( size offset )
{
    nPos += offset;
    if( nPos < 0 ) nPos = 0;
    else if( nPos > iSize ) nPos = iSize;
}

void Bu::StaticMemBuf::setPos( size pos )
{
    nPos = pos;
    if( nPos < 0 ) nPos = 0;
    else if( nPos > iSize ) nPos = iSize;
}

void Bu::StaticMemBuf::setPosEnd( size pos )
{
    nPos = iSize-pos;
    if( nPos < 0 ) nPos = 0;
    else if( nPos > iSize ) nPos = iSize;
}

bool Bu::StaticMemBuf::isEos()
{
    return (nPos == iSize);
}

bool Bu::StaticMemBuf::isOpen()
{
    return true;
}

void Bu::StaticMemBuf::flush()
{
}

bool Bu::StaticMemBuf::canRead()
{
    return !isEos();
}

bool Bu::StaticMemBuf::canWrite()
{
    return false;
}

bool Bu::StaticMemBuf::isReadable()
{
    return true;
}

bool Bu::StaticMemBuf::isWritable()
{
    return false;
}

bool Bu::StaticMemBuf::isSeekable()
{
    return true;
}

bool Bu::StaticMemBuf::isBlocking()
{
    return true;
}

void Bu::StaticMemBuf::setBlocking( bool )
{
}

void Bu::StaticMemBuf::setSize( size )
{
}

Bu::size Bu::StaticMemBuf::getSize() const
{
    return iSize;
}

Bu::size Bu::StaticMemBuf::getBlockSize() const
{
    return iSize;
}

Bu::String Bu::StaticMemBuf::getLocation() const
{
    return "";
}

