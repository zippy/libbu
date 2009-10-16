/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/substream.h"

Bu::SubStream::SubStream( Bu::Stream &rNext, long iSize ) :
	Bu::Filter( rNext ),
	iStart( 0 ),
	iPos( 0 ),
	iSize( iSize )
{
	iStart = rNext.tell();
}

Bu::SubStream::~SubStream()
{
}

size_t Bu::SubStream::read( void *pBuf, size_t nBytes )
{
	if( (long)nBytes > iSize-iPos )
		nBytes = iSize-iPos;
	nBytes = rNext.read( pBuf, nBytes );
	iPos += nBytes;
	return nBytes;
}

size_t Bu::SubStream::write( const void *pBuf, size_t nBytes )
{
	if( (long)nBytes > iSize-iPos )
		nBytes = iSize-iPos;
	nBytes = rNext.write( pBuf, nBytes );
	iPos += nBytes;
	return nBytes;
}

void Bu::SubStream::start()
{
	// doesn't mean anything...
}

size_t Bu::SubStream::stop()
{
	// doesn't mean anything...
	return 0;
}

void Bu::SubStream::close()
{
	// don't do anything?  maybe...
}

long Bu::SubStream::tell()
{
	return iPos;
}

void Bu::SubStream::seek( long offset )
{
	if( iPos+offset < 0 )
		offset = -iPos;
	else if( iPos+offset > iSize )
		offset = iSize-iPos;
	rNext.seek( offset );
	iPos += offset;
}

void Bu::SubStream::setPos( long pos )
{
	if( pos < 0 )
		pos = 0;
	else if( pos > iSize )
		pos = iSize;
	iPos = pos;
	pos += iStart;
	rNext.setPos( pos );
}

void Bu::SubStream::setPosEnd( long pos )
{
	if( iSize-pos < 0 )
		pos = 0;
	else if( iSize-pos > iSize )
		pos = iSize;
	else
		pos = iSize-pos;
	iPos = pos;
	rNext.setPos( iStart+pos );
}

bool Bu::SubStream::isEos()
{
	return rNext.isEos() || iPos == iSize;
}

bool Bu::SubStream::canRead()
{
	return rNext.canRead() && (iPos < iSize);
}

bool Bu::SubStream::canWrite()
{
	return rNext.canWrite() && (iPos < iSize);
}

