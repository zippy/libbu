/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/nullstream.h"

Bu::NullStream::NullStream() :
    sRead( 0 ),
    sWrote( 0 )
{
}

Bu::NullStream::~NullStream()
{
}

void Bu::NullStream::close()
{
    sRead = sWrote = 0;
}

Bu::size Bu::NullStream::read( void *pBuf, Bu::size nBytes )
{
    memset( pBuf, 0, nBytes );
    sRead += nBytes;
    return nBytes;
}

Bu::String Bu::NullStream::readLine()
{
    sRead++;
    return Bu::String("\0", 1 );
}

Bu::size Bu::NullStream::write( const void *, Bu::size nBytes )
{
    sWrote += nBytes;
    return nBytes;
}

Bu::size Bu::NullStream::tell()
{
    return sRead + sWrote;
}

void Bu::NullStream::seek( Bu::size )
{
}

void Bu::NullStream::setPos( Bu::size )
{
}

void Bu::NullStream::setPosEnd( Bu::size )
{
}

bool Bu::NullStream::isEos()
{
    return false;
}

bool Bu::NullStream::isOpen()
{
    return true;
}

void Bu::NullStream::flush()
{
}

bool Bu::NullStream::canRead()
{
    return true;
}

bool Bu::NullStream::canWrite()
{
    return true;
}

bool Bu::NullStream::isReadable()
{
    return true;
}

bool Bu::NullStream::isWritable()
{
    return true;
}

bool Bu::NullStream::isSeekable()
{
    return false;
}

bool Bu::NullStream::isBlocking()
{
    return true;
}

void Bu::NullStream::setBlocking( bool )
{
}

void Bu::NullStream::setSize( Bu::size )
{
}

Bu::size Bu::NullStream::getSize() const
{
    return 0;
}

Bu::size Bu::NullStream::getBlockSize() const
{
    return 0;
}

Bu::String Bu::NullStream::getLocation() const
{
    return "";
}

