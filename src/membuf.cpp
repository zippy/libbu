/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
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

size Bu::MemBuf::read( void *pBuf, size nBytes )
{
	if( (size)sBuf.getSize()-(size)nPos < nBytes )
		nBytes = sBuf.getSize()-nPos;

	memcpy( pBuf, sBuf.getStr()+nPos, nBytes );
	nPos += nBytes;

	return nBytes;
}
	
size Bu::MemBuf::write( const void *pBuf, size nBytes )
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
		size iOver = sBuf.getSize() - nPos;
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

size Bu::MemBuf::tell()
{
	return nPos;
}

void Bu::MemBuf::seek( size offset )
{
	nPos += offset;
	if( nPos < 0 ) nPos = 0;
	else if( nPos > sBuf.getSize() ) nPos = sBuf.getSize();
}

void Bu::MemBuf::setPos( size pos )
{
	nPos = pos;
	if( nPos < 0 ) nPos = 0;
	else if( nPos > sBuf.getSize() ) nPos = sBuf.getSize();
}

void Bu::MemBuf::setPosEnd( size pos )
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
	return true;
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

void Bu::MemBuf::setSize( size iSize )
{
	if( iSize < 0 )
		iSize = 0;
	sBuf.setSize( iSize );
	if( nPos > iSize )
		nPos = iSize;
}

Bu::FString &Bu::MemBuf::getString()
{
	return sBuf;
}

void Bu::MemBuf::setString( const Bu::FString &sNewData )
{
	sBuf = sNewData;
	nPos = 0;
}

