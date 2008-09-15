#include "bu/nidsstream.h"

Bu::NidsStream::NidsStream( Nids &rNids ) :
	rNids( rNids ),
	iPos( 0 )
{
}

Bu::NidsStream::NidsStream( const Bu::NidsStream &rSrc ) :
	rNids( rSrc.rNids ),
	iPos( 0 )
{
	
}

Bu::NidsStream::~NidsStream()
{
}

void Bu::NidsStream::close()
{
}

size_t Bu::NidsStream::read( void *pBuf, size_t nBytes )
{
	return 0;
}

size_t Bu::NidsStream::write( const void *pBuf, size_t nBytes )
{
	return 0;
}

long Bu::NidsStream::tell()
{
	return 0;
}

void Bu::NidsStream::seek( long offset )
{
}

void Bu::NidsStream::setPos( long pos )
{
}

void Bu::NidsStream::setPosEnd( long pos )
{
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

void Bu::NidsStream::setBlocking( bool bBlocking )
{
}

