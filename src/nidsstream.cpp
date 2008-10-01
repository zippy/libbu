#include "bu/nidsstream.h"

Bu::NidsStream::NidsStream( Nids &rNids, uint32_t uStream ) :
	rNids( rNids ),
	uStream( uStream ),
	pCurBlock( NULL ),
	uCurBlock( uStream ),
	uSize( 0 ),
	uBlockSize( rNids.iBlockSize-sizeof(Nids::Block) ),
	uPos( 0 )
{
	printf("NidsStream::allocated\n");
	pCurBlock = rNids.newBlock();
	rNids.getBlock( uStream, pCurBlock );
	uSize = pCurBlock->uBytesUsed;
}

/*
Bu::NidsStream::NidsStream( const Bu::NidsStream &rSrc ) :
	Stream( rSrc ),
	rNids( rSrc.rNids ),
	uStream( rSrc.uStream ),
	pCurBlock( NULL ),
	uCurBlock( uStream ),
	uSize( 0 ),
	uBlockSize( rSrc.uBlockSize ),
	iUsable( uBlockSize-sizeof(Nids::Block) ),
	uPos( 0 )
{
	printf("NidsStream::copied\n");
	pCurBlock = rNids.newBlock();
	rNids.getBlock( uStream, pCurBlock );
}*/

Bu::NidsStream::~NidsStream()
{
	rNids.updateStreamSize( uStream, uSize );
	rNids.deleteBlock( pCurBlock );
}

void Bu::NidsStream::close()
{
}

size_t Bu::NidsStream::read( void *pBuf, size_t nBytes )
{
	if( uPos%uBlockSize+nBytes < uBlockSize )
	{
		size_t iRead = nBytes;
		if( iRead > pCurBlock->uBytesUsed )
			iRead = pCurBlock->uBytesUsed;
		memcpy( pBuf, pCurBlock->pData+(uPos%uBlockSize), iRead );
		uPos += nBytes;
		printf("a: block %u = %ub (%ub total)\n",
			uCurBlock, pCurBlock->uBytesUsed, uSize );
		return iRead;
	}
	else
	{
		//size_t iTotal = 0;
		for(;;)
		{
			
		}
	}
	return 0;
}

size_t Bu::NidsStream::write( const void *pBuf, size_t nBytes )
{
	if( uPos%uBlockSize+nBytes < uBlockSize )
	{
		memcpy( pCurBlock->pData+(uPos%uBlockSize), pBuf, nBytes );
		pCurBlock->uBytesUsed += nBytes;
		rNids.setBlock( uCurBlock, pCurBlock );
		uPos += nBytes;
		uSize += nBytes;
		printf("a: block %u = %ub (%ub total)\n",
			uCurBlock, pCurBlock->uBytesUsed, uSize );
		return nBytes;
	}
	else
	{
		size_t nTotal = 0;
		for(;;)
		{
			uint32_t uNow = uBlockSize-(uPos%uBlockSize);
			if( nBytes < uNow )
				uNow = nBytes;
			memcpy( pCurBlock->pData+(uPos%uBlockSize),
				&((char *)pBuf)[nTotal], uNow );
			pCurBlock->uBytesUsed += uNow;
			rNids.setBlock( uCurBlock, pCurBlock );
			uSize += uNow;
			uPos += uNow;
			nTotal += uNow;
			nBytes -= uNow;
			printf("b: block %u = %ub (%ub total)\n",
				uCurBlock, pCurBlock->uBytesUsed, uSize );
			if( nBytes == 0 )
				return nTotal;
			if( pCurBlock->uBytesUsed == uBlockSize )
				uCurBlock = rNids.getNextBlock( uCurBlock, pCurBlock );
		}
	}
}

long Bu::NidsStream::tell()
{
	return uPos;
}

void Bu::NidsStream::seek( long offset )
{
	uPos += offset;
}

void Bu::NidsStream::setPos( long pos )
{
	uPos = pos;
}

void Bu::NidsStream::setPosEnd( long pos )
{
	uPos = uSize-pos-1;
}

bool Bu::NidsStream::isEOS()
{
	return true;
}

bool Bu::NidsStream::isOpen()
{
	return true;
}

void Bu::NidsStream::flush()
{
}

bool Bu::NidsStream::canRead()
{
	return true;
}

bool Bu::NidsStream::canWrite()
{
	return true;
}

bool Bu::NidsStream::isReadable()
{
	return true;
}

bool Bu::NidsStream::isWritable()
{
	return true;
}

bool Bu::NidsStream::isSeekable()
{
	return true;
}

bool Bu::NidsStream::isBlocking()
{
	return true;
}

void Bu::NidsStream::setBlocking( bool /*bBlocking*/ )
{
}

