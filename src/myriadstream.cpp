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

Bu::MyriadStream::MyriadStream( Bu::Myriad &rMyriad,
		Bu::Myriad::Stream *pStream ) :
	rMyriad( rMyriad ),
	pStream( pStream ),
	pCurBlock( NULL ),
	iPos( 0 )
{
	sio << "MyriadStream: Created, iId=" << pStream->iId << ", iSize="
		<< pStream->iSize << sio.nl;
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
	sio << "MyriadStream: Read: Started, asked to read " << nBytes << "b."
		<< sio.nl;
	if( nBytes <= 0 )
		return 0;
	if( nBytes > pStream->iSize-iPos )
		nBytes = pStream->iSize-iPos;
	int iLeft = nBytes;
	sio << "MyriadStream: Read: Started, going to read " << nBytes << "b."
		<< sio.nl;
	if( pCurBlock == NULL )
	{
		sio << "MyriadStream: Read: No block loaded, loading initial block."
			<< sio.nl;
		pCurBlock = rMyriad.getBlock(
			pStream->aBlocks[iPos/rMyriad.iBlockSize]
			);
	}
	while( iLeft > 0 )
	{
		int iCurBlock = pStream->aBlocks[iPos/rMyriad.iBlockSize];
		if( pCurBlock->iBlockIndex != iCurBlock )
		{
			sio << "MyriadStream: Read: Loading new block " << iCurBlock << "."
				<< sio.nl;
			rMyriad.releaseBlock( pCurBlock );
			pCurBlock = rMyriad.getBlock( iCurBlock );
		}

		int iAmnt = Bu::min(
			rMyriad.iBlockSize - iPos%rMyriad.iBlockSize,
			iLeft
			);
//		if( iLeft > iAmnt )
//			iAmnt = iLeft;
		sio << "MyriadStream: Read: Copying out bytes: "
			<< (iPos%rMyriad.iBlockSize) << " - "
			<< iAmnt
			<< ", " << iLeft << "b left." << sio.nl;
		memcpy(
			pBuf,
			pCurBlock->pData+iPos%rMyriad.iBlockSize,
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
	if( nBytes == 0 )
		return 0;
/*	if( pCurBlock->uBytesUsed >= uBlockSize )
	{
		// We're at the end of our current block, allocate another before we do
		// anything.
		uCurBlock = rMyriad.getNextBlock( uCurBlock, pCurBlock );
	} */
	/*
	if( (iPos%uBlockSize)+nBytes < uBlockSize )
	{
		//printf("wa: %u:%u:%u:%u -> ", iPos, iPos%uBlockSize, uSize, pCurBlock->uBytesUsed );
		memcpy( pCurBlock->pData+(iPos%uBlockSize), pBuf, nBytes );
		//printf("write buffill:  %ub, %u-%u/%u -> %d-%d/%d (a:%u:%u)\n",
		//	nBytes, 0, nBytes-1, nBytes,
		//	iPos, iPos+nBytes-1, uSize, uCurBlock,
		//	pCurBlock->uBytesUsed );
		if( (iPos%uBlockSize)+nBytes > pCurBlock->uBytesUsed )
			pCurBlock->uBytesUsed = (iPos%uBlockSize)+nBytes;
		rMyriad.setBlock( uCurBlock, pCurBlock );
		iPos += nBytes;
		if( iPos > uSize )
			uSize = iPos;
		//printf("block %u = %ub (%ub total) %d:%u\n",
		//	uCurBlock, pCurBlock->uBytesUsed, uSize, nBytes, iPos );
		return nBytes;
	}
	else
	{
		size_t nTotal = 0;
		for(;;)
		{
			uint32_t uNow = uBlockSize-(iPos%uBlockSize);
			//printf("uNow:  %u (%u-(%u%%%u)) %d req\n", uNow, uBlockSize, iPos, uBlockSize, nBytes );
			if( nBytes < uNow )
				uNow = nBytes;
			memcpy( pCurBlock->pData+(iPos%uBlockSize),
				&((char *)pBuf)[nTotal], uNow );
			//printf("write buffill:  %ub, %u-%u/%u -> %d-%d/%d (b:%u:%u)\n",
			//	uNow, nTotal, nTotal+uNow-1, nBytes,
			//	iPos, iPos+uNow-1, uSize, uCurBlock, pCurBlock->uBytesUsed );
			if( (iPos%uBlockSize)+uNow > pCurBlock->uBytesUsed )
				pCurBlock->uBytesUsed = (iPos%uBlockSize)+uNow;
			rMyriad.setBlock( uCurBlock, pCurBlock );
			iPos += uNow;
			if( iPos > uSize )
				uSize = iPos;
			nTotal += uNow;
			nBytes -= uNow;
			//printf("wb: block %u = %ub (%ub total)\n",
			//	uCurBlock, pCurBlock->uBytesUsed, uSize );
			//if( pCurBlock->uBytesUsed == uBlockSize )
			if( iPos%uBlockSize == 0 )
				uCurBlock = rMyriad.getNextBlock( uCurBlock, pCurBlock );
			if( nBytes == 0 )
				return nTotal;
		}
	}*/
	return 0;
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

