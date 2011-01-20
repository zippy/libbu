#include "bu/nullstream.h"

Bu::NullStream::NullStream() :
	sRead( 0 ),
	sWrote( 0 )
{
}

Bu::NullStream::~NullStream()
{
}

void Bu::NullStream::close()
{
	sRead = sWrote = 0;
}

size_t Bu::NullStream::read( void *pBuf, size_t nBytes )
{
	memset( pBuf, 0, nBytes );
	sRead += nBytes;
	return nBytes;
}

Bu::String Bu::NullStream::readLine()
{
	sRead++;
	return Bu::String("\0", 1 );
}

size_t Bu::NullStream::write( const void *, size_t nBytes )
{
	sWrote += nBytes;
	return nBytes;
}

long Bu::NullStream::tell()
{
	return sRead + sWrote;
}

void Bu::NullStream::seek( long )
{
}

void Bu::NullStream::setPos( long )
{
}

void Bu::NullStream::setPosEnd( long )
{
}

bool Bu::NullStream::isEos()
{
	return false;
}

bool Bu::NullStream::isOpen()
{
	return true;
}

void Bu::NullStream::flush()
{
}

bool Bu::NullStream::canRead()
{
	return true;
}

bool Bu::NullStream::canWrite()
{
	return true;
}

bool Bu::NullStream::isReadable()
{
	return true;
}

bool Bu::NullStream::isWritable()
{
	return true;
}

bool Bu::NullStream::isSeekable()
{
	return false;
}

bool Bu::NullStream::isBlocking()
{
	return true;
}

void Bu::NullStream::setBlocking( bool )
{
}

void Bu::NullStream::setSize( long )
{
}

