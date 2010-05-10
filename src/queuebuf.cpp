/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
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

int Bu::QueueBuf::getSize()
{
	return iTotalSize;
}

void Bu::QueueBuf::close()
{
	for( BlockList::iterator i = lBlocks.begin(); i; i++ )
		delete[] *i;
	lBlocks.clear();
	iReadOffset = iWriteOffset = iTotalSize = 0;
}

size_t Bu::QueueBuf::read( void *pRawBuf, size_t nBytes )
{
	if( nBytes <= 0 )
		return 0;

	if( lBlocks.isEmpty() )
		return 0;

	size_t iLeft = nBytes;
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
		size_t iCopy = iBlockSize-iReadOffset;
		if( iLeft < iCopy )
			iCopy = iLeft;
		if( iTotalSize < iCopy )
			iCopy = iTotalSize;
		memcpy( pBuf, pBlock+iReadOffset, iCopy );
		iReadOffset += iCopy;
		iLeft -= iCopy;
		pBuf += iCopy;
		iTotalSize -= iCopy;
		sio << "Read " << iCopy << " bytes, new size: " << iTotalSize << sio.nl;
	}

	return nBytes - iLeft;
}

size_t QueueBuf::peek( void *pBuf, size_t nBytes )
{
	if( nBytes <= 0 )
		return 0;

	if( lBlocks.isEmpty() )
		return 0;

	size_t iLeft = nBytes;
	char *pBuf = (char *)pRawBuf;

	int iTmpReadOffset = iReadOffset;
	int iTmpRemSize = iTotalSize;
	while( iLeft > 0 && iTmpRemSize > 0 )
	{

		// Switching to use temp variables instead of iReadOffset and iTotalSize
		if( iReadOffset == iBlockSize )
		{
			if( lBlocks.isEmpty() )
			{
				return nBytes-iLeft;
			}
			iReadOffset = 0;
		}
		char *pBlock = lBlocks.first();
		size_t iCopy = iBlockSize-iReadOffset;
		if( iLeft < iCopy )
			iCopy = iLeft;
		if( iTotalSize < iCopy )
			iCopy = iTotalSize;
		memcpy( pBuf, pBlock+iReadOffset, iCopy );
		iReadOffset += iCopy;
		iLeft -= iCopy;
		pBuf += iCopy;
		iTotalSize -= iCopy;
		sio << "Read " << iCopy << " bytes, new size: " << iTotalSize << sio.nl;
	}

	return nBytes - iLeft;
}

size_t Bu::QueueBuf::write( const void *pRawBuf, size_t nBytes )
{
	if( nBytes <= 0 )
		return 0;

	if( lBlocks.isEmpty() )
	{
		addBlock();
		iWriteOffset = 0;
	}
	size_t iLeft = nBytes;
	const char *pBuf = (const char *)pRawBuf;

	while( iLeft > 0 )
	{
		if( iWriteOffset == iBlockSize )
		{
			addBlock();
			iWriteOffset = 0;
		}
		char *pBlock = lBlocks.last();
		size_t iCopy = iBlockSize-iWriteOffset;
		if( iLeft < iCopy )
			iCopy = iLeft;
		memcpy( pBlock+iWriteOffset, pBuf, iCopy );
		iWriteOffset += iCopy;
		iLeft -= iCopy;
		pBuf += iCopy;
		iTotalSize += iCopy;
		sio << "Wrote " << iCopy << " bytes, new size: " << iTotalSize << sio.nl;
	}

	return nBytes;
}

long Bu::QueueBuf::tell()
{
	return -1;
}

void Bu::QueueBuf::seek( long )
{
}

void Bu::QueueBuf::setPos( long )
{
}

void Bu::QueueBuf::setPosEnd( long )
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

void Bu::QueueBuf::addBlock()
{
	lBlocks.append( new char[iBlockSize] );
	sio << "Added new block." << sio.nl;
}

void Bu::QueueBuf::removeBlock()
{
	delete[] lBlocks.first();
	lBlocks.erase( lBlocks.begin() );
	sio << "Removed block." << sio.nl;
}

