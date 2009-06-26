#include "bu/buffer.h"

Bu::Buffer::Buffer( Bu::Stream &rNext, int iBufsize ) :
	Bu::Filter( rNext ),
	sSoFar( 0 ),
	iBufSize( iBufSize ),
	sReadBuf( NULL ),
	sWriteBuf( NULL ),
	iReadBufFill( 0 ),
	iWriteBufFill( 0 )
{
}

Bu::Buffer::~Buffer()
{
}

void Bu::Buffer::start()
{
}

size_t Bu::Buffer::stop()
{
}

size_t Bu::Buffer::read( void *pBuf, size_t nBytes )
{
}

size_t Bu::Buffer::write( const void *pBuf, size_t nBytes )
{
}

void Bu::Buffer::flush()
{
}

