/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/base64.h"

namespace Bu { subExceptionDef( Base64Exception ) }

const char Bu::Base64::tblEnc[65] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};

Bu::Base64::Base64( Bu::Stream &rNext ) :
	Bu::Filter( rNext ),
	iBPos( 0 ),
	iBuf( 0 ),
	iRPos( 0 ),
	iChars( 0 ),
	bEosIn( false ),
	iTotalIn( 0 ),
	iTotalOut( 0 ),
	eMode( Nothing )
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
//	if( eMode |= Encode )
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
//	else
//	{
		return iTotalIn;
//	}
}

size_t Bu::Base64::read( void *pBuf, size_t nBytes )
{
	if( bEosIn == true && iRPos == iChars )
		return 0;
	size_t sIn = 0;
	char buf[4];
	while( sIn < nBytes )
	{
		for(; iRPos < iChars && sIn < nBytes; iRPos++, sIn++ )
		{
			((unsigned char *)pBuf)[sIn] = (iBuf>>(8*(2-iRPos)))&0xFF;
		}
		if( iRPos == iChars )
		{
			if( bEosIn == true )
				return sIn;
			else
				iRPos = 0;
		}
		else if( sIn == nBytes )
			return sIn;
		//if( rNext.read( buf, 4 ) == 0 )
		//	return sIn;
		for( int j = 0; j < 4; j++ )
		{
			if( rNext.read( &buf[j], 1 ) == 0 )
			{
				if( rNext.isEos() )
				{
					bEosIn = true;
					if( j != 0 )
					{
						throw Base64Exception(
							"Premature end of stream detected while "
							"decoding Base64 data."
							);
					}
				}
				return sIn;
			}
			if( buf[j] == ' ' || buf[j] == '\t' ||
				buf[j] == '\n' || buf[j] == '\r' )
			{
				j--;
			}
		}
		iChars = 3;
		iBuf = 0;
		for( int j = 0; j < 4; j++ )
		{
			if( buf[j] == '=' )
			{
				iChars--;
				bEosIn = true;
			}
			else
				iBuf |= (tblDec[buf[j]-'+']&0x3f)<<((3-j)*6);
			//printf("%d:  %06X (%02X)\n", j, iBuf, (tblDec[buf[j]-'+']&0x3f) );
		}
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

bool Bu::Base64::isEos()
{
	if( bEosIn == true && iRPos == iChars )
		return true;
	return false;
}

