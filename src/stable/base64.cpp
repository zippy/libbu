/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/base64.h"

namespace Bu { subExceptionDef( Base64Exception ) }

const char Bu::Base64::tblEnc[65] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};

Bu::Base64::Base64( Bu::Stream &rNext, int iChunkSize ) :
	Bu::Filter( rNext ),
	iBPos( 0 ),
	iBuf( 0 ),
	iRPos( 0 ),
	iChars( 0 ),
	bEosIn( false ),
	iTotalIn( 0 ),
	iTotalOut( 0 ),
	eMode( Nothing ),
	iChunkSize( iChunkSize ),
	iCurChunk( 0 )
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
	iCurChunk = 0;
}

Bu::size Bu::Base64::stop()
{
	if( eMode == Encode )
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
		iCurChunk += 4;
		if( iChunkSize && iCurChunk >= iChunkSize )
		{
			iCurChunk = iCurChunk-iChunkSize;
			iTotalOut += rNext.write( outBuf, 4-iCurChunk );
			iTotalOut += rNext.write("\r\n", 2 );
			iTotalOut += rNext.write( outBuf+(4-iCurChunk), iCurChunk );
		}
		else
			iTotalOut += rNext.write( outBuf, 4 );
		return iTotalOut;
	}
	else
	{
		return iTotalIn;
	}
}

Bu::size Bu::Base64::read( void *pBuf, Bu::size nBytes )
{
	if( eMode == Encode )
		throw Bu::Base64Exception("Cannot read from an output stream.");
	eMode = Decode;

	if( bEosIn == true && iRPos == iChars )
		return 0;
	Bu::size sIn = 0;
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
					if( iRPos == 0 )
						iRPos = iChars;
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

Bu::size Bu::Base64::write( const void *pBuf, Bu::size nBytes )
{
	if( eMode == Decode )
		throw Bu::Base64Exception("Cannot write to an input stream.");
	eMode = Encode;

	Bu::size sOut = 0;
	char outBuf[4];
	for( Bu::size j = 0; j < nBytes; j++ )
	{
		iBuf |= (((uint8_t *)pBuf)[j])<<((2-iBPos++)*8);
		if( iBPos == 3 )
		{
			for( int k = 0; k < 4; k++ )
			{
				outBuf[3-k] = tblEnc[(iBuf>>(6*k))&0x3f];
			}
			iCurChunk += 4;
			if( iChunkSize && iCurChunk >= iChunkSize )
			{
				iCurChunk = iCurChunk-iChunkSize;
				sOut += rNext.write( outBuf, 4-iCurChunk );
				sOut += rNext.write("\r\n", 2 );
				sOut += rNext.write( outBuf+(4-iCurChunk), iCurChunk );
			}
			else
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

