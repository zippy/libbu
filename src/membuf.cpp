/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/membuf.h"

using namespace Bu;

Bu::MemBuf::MemBuf() :
	nPos( 0 )
{
}

Bu::MemBuf::MemBuf( const Bu::FString &str ) :
	sBuf( str ),
	nPos( 0 )
{
}

Bu::MemBuf::~MemBuf()
{
}

void Bu::MemBuf::close()
{
}

size_t Bu::MemBuf::read( void *pBuf, size_t nBytes )
{
	if( (size_t)sBuf.getSize()-(size_t)nPos < nBytes )
		nBytes = sBuf.getSize()-nPos;

	memcpy( pBuf, sBuf.getStr()+nPos, nBytes );
	nPos += nBytes;

	return nBytes;
}
	
size_t Bu::MemBuf::write( const void *pBuf, size_t nBytes )
{
	if( nPos == sBuf.getSize() )
	{
		// Easiest, just append the data.
		sBuf.append( (const char *)pBuf, nBytes );
		nPos += nBytes;
		return nBytes;
	}
	else
	{
		// Trickier, we must do this in two parts, overwrite, then append
		// Frist, overwrite.
		size_t iOver = sBuf.getSize() - nPos;
		if( iOver > nBytes )
			iOver = nBytes;
		memcpy( sBuf.getStr()+nPos, pBuf, iOver );
		// Then append
		if( iOver < nBytes )
		{
			sBuf.append( ((const char *)pBuf)+iOver, nBytes-iOver );
		}
		nPos += nBytes;
		return nBytes;
	}
}

long Bu::MemBuf::tell()
{
	return nPos;
}

void Bu::MemBuf::seek( long offset )
{
	nPos += offset;
	if( nPos < 0 ) nPos = 0;
	else if( nPos > sBuf.getSize() ) nPos = sBuf.getSize();
}

void Bu::MemBuf::setPos( long pos )
{
	nPos = pos;
	if( nPos < 0 ) nPos = 0;
	else if( nPos > sBuf.getSize() ) nPos = sBuf.getSize();
}

void Bu::MemBuf::setPosEnd( long pos )
{
	nPos = sBuf.getSize()-pos;
	if( nPos < 0 ) nPos = 0;
	else if( nPos > sBuf.getSize() ) nPos = sBuf.getSize();
}

bool Bu::MemBuf::isEos()
{
	return (nPos == sBuf.getSize());
}

bool Bu::MemBuf::isOpen()
{
	return true;
}

void Bu::MemBuf::flush()
{
}

bool Bu::MemBuf::canRead()
{
	return !isEos();
}

bool Bu::MemBuf::canWrite()
{
	return isEos();
}

bool Bu::MemBuf::isReadable()
{
	return true;
}

bool Bu::MemBuf::isWritable()
{
	return true;
}

bool Bu::MemBuf::isSeekable()
{
	return true;
}

bool Bu::MemBuf::isBlocking()
{
	return true;
}

void Bu::MemBuf::setBlocking( bool )
{
}

Bu::FString &Bu::MemBuf::getString()
{
	return sBuf;
}

