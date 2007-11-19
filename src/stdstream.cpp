/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/stdstream.h"

Bu::StdStream::StdStream()
{
}

Bu::StdStream::~StdStream()
{
}

void Bu::StdStream::close()
{
}

size_t Bu::StdStream::read( void *pBuf, size_t nBytes )
{
	return fread( pBuf, 1, nBytes, stdin );
}

size_t Bu::StdStream::write( const void *pBuf, size_t nBytes )
{
	return fwrite( pBuf, 1, nBytes, stdout );
}

long Bu::StdStream::tell()
{
	return 0;
}

void Bu::StdStream::seek( long offset )
{
}

void Bu::StdStream::setPos( long pos )
{
}

void Bu::StdStream::setPosEnd( long pos )
{
}

bool Bu::StdStream::isEOS()
{
	return false;
}

bool Bu::StdStream::isOpen()
{
	return true;
}

void Bu::StdStream::flush()
{
	fflush( stdout );
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

void Bu::StdStream::setBlocking( bool bBlocking )
{
}

