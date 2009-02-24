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

