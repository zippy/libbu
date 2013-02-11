/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include "bu/stdstream.h"

Bu::StdStream::StdStream( OutMode eOut ) :
    eOut( eOut )
{
}

Bu::StdStream::~StdStream()
{
}

void Bu::StdStream::close()
{
}

Bu::size Bu::StdStream::read( void *pBuf, Bu::size nBytes )
{
    return fread( pBuf, 1, nBytes, stdin );
}

Bu::size Bu::StdStream::write( const void *pBuf, Bu::size nBytes )
{
    return fwrite( pBuf, 1, nBytes, eOut==StdOut?stdout:stderr );
}

Bu::size Bu::StdStream::tell()
{
    return 0;
}

void Bu::StdStream::seek( Bu::size )
{
}

void Bu::StdStream::setPos( Bu::size )
{
}

void Bu::StdStream::setPosEnd( Bu::size )
{
}

bool Bu::StdStream::isEos()
{
    return false;
}

bool Bu::StdStream::isOpen()
{
    return true;
}

void Bu::StdStream::flush()
{
    fflush( eOut==StdOut?stdout:stderr );
}

bool Bu::StdStream::canRead()
{
    return true;
}

bool Bu::StdStream::canWrite()
{
    return true;
}

bool Bu::StdStream::isReadable()
{
    return true;
}

bool Bu::StdStream::isWritable()
{
    return true;
}

bool Bu::StdStream::isSeekable()
{
    return false;
}

bool Bu::StdStream::isBlocking()
{
    return true;
}

void Bu::StdStream::setBlocking( bool )
{
}

void Bu::StdStream::setSize( Bu::size )
{
}

Bu::size Bu::StdStream::getSize() const
{
    return 0;
}

Bu::size Bu::StdStream::getBlockSize() const
{
    return 0;
}

Bu::String Bu::StdStream::getLocation() const
{
    return "";
}

