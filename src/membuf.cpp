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
	sBuf.append( (const char *)pBuf, nBytes );
	nPos += nBytes;
	return nBytes;
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

bool Bu::MemBuf::isEOS()
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
	return !isEOS();
}

bool Bu::MemBuf::canWrite()
{
	return isEOS();
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

void Bu::MemBuf::setBlocking( bool bBlocking )
{
}

Bu::FString &Bu::MemBuf::getString()
{
	return sBuf;
}
