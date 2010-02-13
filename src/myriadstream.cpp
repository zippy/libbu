/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/myriadstream.h"

#include <string.h>

Bu::MyriadStream::MyriadStream( Myriad &rMyriad, uint32_t uStream ) :
	rMyriad( rMyriad ),
	uStream( uStream ),
	pCurBlock( NULL ),
	uCurBlock( uStream ),
	uSize( 0 ),
	uBlockSize( rMyriad.iBlockSize ),
	uPos( 0 )
{
	//printf("MyriadStream::allocated\n");
	//pCurBlock = rMyriad.newBlock();
	//rMyriad.getBlock( uStream, pCurBlock );
	//uSize = pCurBlock->uBytesUsed;
}

Bu::MyriadStream::~MyriadStream()
{
	//printf("Destroying stream?\n");
	//rMyriad.updateStreamSize( uStream, uSize );
	//rMyriad.deleteBlock( pCurBlock );
}

void Bu::MyriadStream::close()
{
}

size_t Bu::MyriadStream::read( void *pBuf, size_t nBytes )
{
	/*
	if( nBytes == 0 )
		return 0;
	if( nBytes + uPos > uSize )
		nBytes = uSize - uPos;
	if( (uPos%uBlockSize)+nBytes < uBlockSize )
	{
		size_t iRead = nBytes;
		if( iRead > pCurBlock->uBytesUsed-(uPos%uBlockSize) )
			iRead = pCurBlock->uBytesUsed-(uPos%uBlockSize);
		memcpy( pBuf, pCurBlock->pData+(uPos%uBlockSize), iRead );
		//printf("read buffill:  %ub, %u-%u/%u -> %d-%d/%d (a:%u)",
		//	iRead, uPos, uPos+iRead-1, uSize, 0, iRead-1, nBytes, uCurBlock );
		uPos += iRead;
		//printf(" -- %u\n", uPos%uBlockSize );
		//printf("ra: block %u = %ub:%u (%ub total)\n",
		//	uCurBlock, uPos, nBytes, uSize );

		// This can't happen, if we're right on a boundery, it goes to the
		// other case
		//if( uPos%uBlockSize == 0 )
		//	uCurBlock = rMyriad.getNextBlock( uCurBlock, pCurBlock, false );
		return iRead;
	}
	else
	{
		size_t nTotal = 0;
		for(;;)
		{
			uint32_t iRead = nBytes;
			if( iRead > uBlockSize-(uPos%uBlockSize) )
				iRead = uBlockSize-(uPos%uBlockSize);
			if( iRead > pCurBlock->uBytesUsed-(uPos%uBlockSize) )
				iRead = pCurBlock->uBytesUsed-(uPos%uBlockSize);
			memcpy( ((char *)pBuf)+nTotal,
				pCurBlock->pData+(uPos%uBlockSize), iRead );
			//printf(" read buffill:  %ub, %u-%u/%u -> %d-%d/%d (b:%u)\n",
			//	iRead, uPos, uPos+iRead-1, uSize,
			//	nTotal, nTotal+nBytes-1, nBytes, uCurBlock );
			uPos += iRead;
			nBytes -= iRead;
			nTotal += iRead;
			//printf("rb: block %u = %ub:%u (%ub total)\n",
			//	uCurBlock, uPos, iRead, uSize );
			if( uPos%uBlockSize == 0 )
				uCurBlock = rMyriad.getNextBlock( uCurBlock, pCurBlock, false );
			if( nBytes == 0 || uPos >= uSize )
				return nTotal;
		}
	}*/
	return 0;
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
	if( (uPos%uBlockSize)+nBytes < uBlockSize )
	{
		//printf("wa: %u:%u:%u:%u -> ", uPos, uPos%uBlockSize, uSize, pCurBlock->uBytesUsed );
		memcpy( pCurBlock->pData+(uPos%uBlockSize), pBuf, nBytes );
		//printf("write buffill:  %ub, %u-%u/%u -> %d-%d/%d (a:%u:%u)\n",
		//	nBytes, 0, nBytes-1, nBytes,
		//	uPos, uPos+nBytes-1, uSize, uCurBlock,
		//	pCurBlock->uBytesUsed );
		if( (uPos%uBlockSize)+nBytes > pCurBlock->uBytesUsed )
			pCurBlock->uBytesUsed = (uPos%uBlockSize)+nBytes;
		rMyriad.setBlock( uCurBlock, pCurBlock );
		uPos += nBytes;
		if( uPos > uSize )
			uSize = uPos;
		//printf("block %u = %ub (%ub total) %d:%u\n",
		//	uCurBlock, pCurBlock->uBytesUsed, uSize, nBytes, uPos );
		return nBytes;
	}
	else
	{
		size_t nTotal = 0;
		for(;;)
		{
			uint32_t uNow = uBlockSize-(uPos%uBlockSize);
			//printf("uNow:  %u (%u-(%u%%%u)) %d req\n", uNow, uBlockSize, uPos, uBlockSize, nBytes );
			if( nBytes < uNow )
				uNow = nBytes;
			memcpy( pCurBlock->pData+(uPos%uBlockSize),
				&((char *)pBuf)[nTotal], uNow );
			//printf("write buffill:  %ub, %u-%u/%u -> %d-%d/%d (b:%u:%u)\n",
			//	uNow, nTotal, nTotal+uNow-1, nBytes,
			//	uPos, uPos+uNow-1, uSize, uCurBlock, pCurBlock->uBytesUsed );
			if( (uPos%uBlockSize)+uNow > pCurBlock->uBytesUsed )
				pCurBlock->uBytesUsed = (uPos%uBlockSize)+uNow;
			rMyriad.setBlock( uCurBlock, pCurBlock );
			uPos += uNow;
			if( uPos > uSize )
				uSize = uPos;
			nTotal += uNow;
			nBytes -= uNow;
			//printf("wb: block %u = %ub (%ub total)\n",
			//	uCurBlock, pCurBlock->uBytesUsed, uSize );
			//if( pCurBlock->uBytesUsed == uBlockSize )
			if( uPos%uBlockSize == 0 )
				uCurBlock = rMyriad.getNextBlock( uCurBlock, pCurBlock );
			if( nBytes == 0 )
				return nTotal;
		}
	}*/
	return 0;
}

long Bu::MyriadStream::tell()
{
	return uPos;
}

void Bu::MyriadStream::seek( long offset )
{
	uPos += offset;
}

void Bu::MyriadStream::setPos( long pos )
{
	uPos = pos;
}

void Bu::MyriadStream::setPosEnd( long pos )
{
	uPos = uSize-pos-1;
}

bool Bu::MyriadStream::isEos()
{
	return true;
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

