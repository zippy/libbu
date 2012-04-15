/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/hex.h"

Bu::Hex::Hex( Bu::Stream &rNext, bool bUpperCase, int iChunk ) :
	Bu::Filter( rNext ),
	iChunk( iChunk ),
	iPos( 0 ),
	iIn( 0 ),
	sChrs(bUpperCase?"0123456789ABCDEF":"0123456789abcdef")
{
}

Bu::Hex::~Hex()
{
}

void Bu::Hex::start()
{
	iPos = iIn = 0;
}

Bu::size Bu::Hex::stop()
{
	return iPos;
}

Bu::size Bu::Hex::read( void *pBuf, Bu::size iBytes )
{
	Bu::size j;
	uint8_t *puBuf = (uint8_t *)pBuf;
	for( j = 0; j < iBytes; j++ )
	{
		for(; iIn < 2; iIn++ )
		{
			if( rNext.read( &cIn[iIn], 1 ) == 0 )
				return j;
			if( cIn[iIn] == ' ' || cIn[iIn] == '\t' ||
				cIn[iIn] == '\n' || cIn[iIn] == '\r' )
				iIn--;
		}
#define chr2nibble( c ) ((c>='0'&&c<='9')?(c-'0'):((c|0x60)-'a'+10))
		puBuf[j] = ((chr2nibble(cIn[0])<<4)|chr2nibble(cIn[1]));
		iIn = 0;
	}
	return j;
}

Bu::size Bu::Hex::write( const void *pBuf, Bu::size iBytes )
{
	char cOut[2];
	uint8_t *puBuf = (uint8_t *)pBuf;
	for( Bu::size j = 0; j < iBytes; j++ )
	{
		cOut[0] = sChrs[(puBuf[j]&0xf0)>>4];
		cOut[1] = sChrs[(puBuf[j]&0x0f)];
		if( iChunk > 0 && iPos%iChunk == 0 && iPos>0 )
			rNext.write(" ", 1 );
		rNext.write( cOut, 2 );
		iPos++;
	}
	return iBytes;
}

