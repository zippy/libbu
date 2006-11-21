#include <string.h>
#include "sbuffer.h"

SBuffer::SBuffer() :
	nPos( 0 ),
	bOpen( true )
{
}

SBuffer::~SBuffer()
{
}

void SBuffer::close()
{
	bOpen = false;
	fbData.clearData();
}

size_t SBuffer::read( char *pBuf, size_t nBytes )
{
	size_t nLeft = fbData.getLength() - nPos;
	if( nBytes > nLeft )
		nBytes = nLeft;

	if( nLeft == 0 )
		return 0;

	memcpy( pBuf, fbData.getData()+nPos, nBytes );
	nPos += nBytes;

	return nBytes;
}

size_t SBuffer::write( const char *pBuf, size_t nBytes )
{
	fbData.appendData( pBuf, nBytes );
	nPos += nBytes;

	return nBytes;
}

long SBuffer::tell()
{
	return nPos;
}

void SBuffer::seek( long offset )
{
	nPos += offset;
}

void SBuffer::setPos( long pos )
{
	nPos = pos;
}

void SBuffer::setPosEnd( long pos )
{
	nPos = fbData.getLength() - pos;
}

