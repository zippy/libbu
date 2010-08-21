#include "bu/streamstack.h"

Bu::StreamStack::StreamStack()
{
}

Bu::StreamStack::~StreamStack()
{
}

void Bu::StreamStack::close()
{
}

size_t Bu::StreamStack::read( void *pBuf, size_t nBytes )
{
}

size_t Bu::StreamStack::write( const void *pBuf, size_t nBytes )
{
}

size_t Bu::StreamStack::write( const Bu::FString &sBuf )
{
}

long Bu::StreamStack::tell()
{
}

void Bu::StreamStack::seek( long offset )
{
}

void Bu::StreamStack::setPos( long pos )
{
}

void Bu::StreamStack::setPosEnd( long pos )
{
}

bool Bu::StreamStack::isEos()
{
}

bool Bu::StreamStack::isOpen()
{
}

void Bu::StreamStack::flush()
{
}

bool Bu::StreamStack::canRead()
{
}

bool Bu::StreamStack::canWrite()
{
}

bool Bu::StreamStack::isReadable()
{
}

bool Bu::StreamStack::isWritable()
{
}

bool Bu::StreamStack::isSeekable()
{
}

bool Bu::StreamStack::isBlocking()
{
}

void Bu::StreamStack::setBlocking( bool bBlocking )
{
}

void Bu::StreamStack::setSize( long iSize )
{
}

