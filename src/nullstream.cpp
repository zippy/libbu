#include "bu/nullstream.h"

Bu::NullStream::NullStream()
{
}

Bu::NullStream::~NullStream()
{
}

void Bu::NullStream::close();
size_t Bu::NullStream::read( void *pBuf, size_t nBytes );
Bu::FString Bu::NullStream::readLine();
size_t Bu::NullStream::write( const void *pBuf, size_t nBytes );
long Bu::NullStream::tell();
void Bu::NullStream::seek( long offset );
void Bu::NullStream::setPos( long pos );
void Bu::NullStream::setPosEnd( long pos );
bool Bu::NullStream::isEos();
bool Bu::NullStream::isOpen();
void Bu::NullStream::flush();
bool Bu::NullStream::canRead();
bool Bu::NullStream::canWrite();
bool Bu::NullStream::isReadable();
bool Bu::NullStream::isWritable();
bool Bu::NullStream::isSeekable();
bool Bu::NullStream::isBlocking();
void Bu::NullStream::setBlocking( bool bBlocking=true );
void Bu::NullStream::setSize( long iSize );


