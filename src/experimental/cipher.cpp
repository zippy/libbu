#include "bu/cipher.h"

Bu::Cipher::Cipher( Bu::Stream &rNext ) :
	Bu::Filter( rNext )
{
}

Bu::Cipher::~Cipher()
{
}

void Bu::Cipher::start()
{
}

Bu::size Bu::Cipher::stop()
{
	return 0;
}

Bu::size Bu::Cipher::read( void *pBuf, Bu::size iBytes )
{
	uint32_t i;

	if (iBytes%8)
	{
		return 0;
	}

	iBytes /= 8;

	for (i=0;i<iBytes;i++)
	{
		void *pSeg = ((char *)pBuf)+(i*8);
		int iRead = rNext.read( pSeg, 8 );
		decipher( pSeg );
	}

	return iBytes*8;
}

Bu::size Bu::Cipher::write( const void *pBuf, Bu::size iBytes )
{
	uint32_t i;

	if (iBytes%8)
	{
		return 0;
	}

	iBytes /= 8;

	char buf[8];

	for (i=0;i<iBytes;i++)
	{
		memcpy( buf, ((const char *)pBuf)+(i*8), 8 );
		encipher( buf );
		rNext.write( buf, 8 );
	}

	memset( &buf, 0, 8 );
	return iBytes*8;
}

