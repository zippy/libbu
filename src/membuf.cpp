#include "bu/membuf.h"

using namespace Bu;

Bu::MemBuf::MemBuf()
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

}
	
size_t Bu::MemBuf::write( const void *pBuf, size_t nBytes )
{
}

long Bu::MemBuf::tell()
{
}

void Bu::MemBuf::seek( long offset )
{
}

void Bu::MemBuf::setPos( long pos )
{
}

void Bu::MemBuf::setPosEnd( long pos )
{
}

bool Bu::MemBuf::isEOS()
{
}

bool Bu::MemBuf::isOpen()
{
}

void Bu::MemBuf::flush()
{
}

bool Bu::MemBuf::canRead()
{
}

bool Bu::MemBuf::canWrite()
{
}

bool Bu::MemBuf::isReadable()
{
}

bool Bu::MemBuf::isWritable()
{
}

bool Bu::MemBuf::isSeekable()
{
}

bool Bu::MemBuf::isBlocking()
{
}

void Bu::MemBuf::setBlocking( bool bBlocking )
{
}

