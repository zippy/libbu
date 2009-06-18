#include "bu/base64.h"

const char Bu::Base64::tblEnc[65] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};

Bu::Base64::Base64( Bu::Stream &rNext, Mode eMode ) :
	Bu::Filter( rNext ),
	iBPos( 0 ),
	iBuf( 0 ),
	iTotalIn( 0 ),
	iTotalOut( 0 ),
	eMode( eMode )
{
	start();

	memset( tblDec, 0, 80 );
	for( int j = 0; j < 64; j++ )
	{
		tblDec[tblEnc[j]-'+'] = j;
	//	printf("'%c' = %d\n", tblEnc[j], j );
	}
	/*
	for( int j = 0; j < 64; j++ )
	{
		printf("'%c' = '%c' (%d = %d)\n",
			tblEnc[j], tblEnc[tblDec[tblEnc[j]-'+']],
			j, tblDec[tblEnc[j]-'+'] );
	}*/

	// The following is used to compute the table size for the decoding table.
	/*
	char low='A', high='A';
	for( int j = 0; j < 64; j++ )
	{
		if( tblEnc[j] < low )
			low = tblEnc[j];
		if( tblEnc[j] > high )
			high = tblEnc[j];
	}

	printf("'%c' - '%c' (%d - %d) (%d)\n", low, high, low, high, high-low );
	*/
}

Bu::Base64::~Base64()
{
	stop();
}

void Bu::Base64::start()
{
}

size_t Bu::Base64::stop()
{
	if( eMode == Write )
	{
		char outBuf[4];
		int iBUsed = 4-(3-iBPos);
		if( iBPos == 0 )
			return iTotalOut;
		for( int k = 0; k < 4; k++ )
		{
			outBuf[3-k] = tblEnc[(iBuf>>(6*k))&0x3f];
		}
		for( int k = iBUsed; k < 4; k++ )
		{
			outBuf[k] = '=';
		}
		iTotalOut += rNext.write( outBuf, 4 );
		return iTotalOut;
	}
	else
	{
		return iTotalIn;
	}
}

size_t Bu::Base64::read( void *pBuf, size_t nBytes )
{
	size_t sIn = 0;
	char buf[4];
	while( sIn < nBytes )
	{
		if( rNext.read( buf, 4 ) == 0 )
			return sIn;
		int iChars = 3;
		for( int j = 0; j < 4; j++ )
		{
			if( buf[j] == '=' )
				iChars--;
			else
				iBuf |= (tblDec[buf[j]-'+']&0x3f)<<((3-j)*6);
			printf("%d:  %06X (%02X)\n", j, iBuf, (tblDec[buf[j]-'+']&0x3f) );
		}
		for( int j = 0; j < iChars; j++ )
		{
			((unsigned char *)pBuf)[sIn++] = (iBuf>>(8*(2-j)))&0xFF;
		}
		iBuf = 0;
	}

	return sIn;
}

size_t Bu::Base64::write( const void *pBuf, size_t nBytes )
{
	size_t sOut = 0;
	char outBuf[4];
	for( size_t j = 0; j < nBytes; j++ )
	{
		iBuf |= (((uint8_t *)pBuf)[j])<<((2-iBPos++)*8);
		if( iBPos == 3 )
		{
			for( int k = 0; k < 4; k++ )
			{
				outBuf[3-k] = tblEnc[(iBuf>>(6*k))&0x3f];
			}
			sOut += rNext.write( outBuf, 4 );
			iBPos = iBuf = 0;
		}
	}
	iTotalOut += sOut;
	return sOut;
}

bool Bu::Base64::isOpen()
{
	return true;
}

