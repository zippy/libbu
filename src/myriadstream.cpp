/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/myriadstream.h"
#include "bu/sio.h"

using Bu::sio;
using Bu::Fmt;

#include <string.h>

// #define MYRIAD_STREAM_DEBUG	1

Bu::MyriadStream::MyriadStream( Bu::Myriad &rMyriad,
		Bu::Myriad::Stream *pStream ) :
	rMyriad( rMyriad ),
	pStream( pStream ),
	pCurBlock( NULL ),
	iPos( 0 )
{
#ifdef MYRIAD_STREAM_DEBUG
	sio << "MyriadStream: Created, iId=" << pStream->iId << ", iSize="
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

size_t Bu::MyriadStream::read( void *pBuf, size_t nBytes )
{
#ifdef MYRIAD_STREAM_DEBUG
	sio << "MyriadStream: read: Started, asked to read " << nBytes << "b."
		<< sio.nl;
#endif
	if( nBytes > pStream->iSize-iPos )
		nBytes = pStream->iSize-iPos;
	if( nBytes <= 0 )
		return 0;
	int iLeft = nBytes;
#ifdef MYRIAD_STREAM_DEBUG
	sio << "MyriadStream: read: Started, going to read " << nBytes << "b."
		<< sio.nl;
#endif
	if( pCurBlock == NULL )
	{
#ifdef MYRIAD_STREAM_DEBUG
		sio << "MyriadStream: read: No block loaded, loading initial block."
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
			sio << "MyriadStream: read: Loading new block " << iCurBlock << "."
				<< sio.nl;
#endif
			rMyriad.releaseBlock( pCurBlock );
			pCurBlock = rMyriad.getBlock( iCurBlock );
		}

		int iAmnt = Bu::min(
			Bu::min(
				rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
				iLeft
				),
			pStream->iSize-iPos
			);
#ifdef MYRIAD_STREAM_DEBUG
		sio << "MyriadStream: read: Copying out bytes: "
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

size_t Bu::MyriadStream::write( const void *pBuf, size_t nBytes )
{
	if( nBytes <= 0 )
		return 0;

#ifdef MYRIAD_STREAM_DEBUG
	sio << "MyriadStream: write: Started, asked to write " << nBytes << "b."
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
			iCurBlock = rMyriad.findEmptyBlock();
			pStream->aBlocks.append( iCurBlock );
			rMyriad.bsBlockUsed.setBit( iCurBlock );
#ifdef MYRIAD_STREAM_DEBUG
			sio << "MyriadStream: write: New block allocated and appended: "
				<< iCurBlock << "." << sio.nl;

#endif
		}
		if( !pCurBlock || pCurBlock->iBlockIndex != iCurBlock )
		{
#ifdef MYRIAD_STREAM_DEBUG
			sio << "MyriadStream: write: Loading new block " << iCurBlock << "."
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
			int iAmnt = Bu::min(
				Bu::min(
					rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
					iLeft
					),
				pStream->iSize-iPos
				);
#ifdef MYRIAD_STREAM_DEBUG
			sio << "MyriadStream: write (ovr): Copying in bytes: "
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
			int iAmnt = Bu::min(
				rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
				iLeft
				);
#ifdef MYRIAD_STREAM_DEBUG
			sio << "MyriadStream: write (app): Copying in bytes: "
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
			pBuf = &((char *)pBuf)[iAmnt];
			iLeft -= iAmnt;
		}
	}

	return nBytes;
}

long Bu::MyriadStream::tell()
{
	return iPos;
}

void Bu::MyriadStream::seek( long offset )
{
	iPos += offset;
}

void Bu::MyriadStream::setPos( long pos )
{
	iPos = pos;
}

void Bu::MyriadStream::setPosEnd( long pos )
{
	iPos = pStream->iSize-pos-1;
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

