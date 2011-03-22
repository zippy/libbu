/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/utfstring.h"

#include "bu/string.h"

Bu::UtfString::UtfString()
{
}

Bu::UtfString::~UtfString()
{
}

#include "bu/sio.h"
using Bu::sio;

void Bu::UtfString::debugUtf8( const Bu::String &sUtf8 )
{
	static uint8_t lmask[8] = {
		0x00,
		0x01,
		0x03,
		0x07,
		0x0f,
		0x1f,
		0x3f,
		0x7f
	};
	for( Bu::String::const_iterator i = sUtf8.begin(); i; i++ )
	{
		if( i != sUtf8.begin() )
			sio << ", ";
		if( ((int)(uint8_t)*i)&0x80 )
		{
//			sio << "Flag byte: " << Bu::Fmt().radix(2).width(8).fill('0')
//				<< (int)(uint8_t)*i << sio.nl;
			int iBytes = 1;
			for(; (((uint8_t)(*i))<<iBytes)&0x80; iBytes++ ) { }
//			sio << "iBytes = " << iBytes << sio.nl;
			point uPt = ((*i) & lmask[7-iBytes])<<(6*(iBytes-1));
//			sio << "mask: " << Bu::Fmt().radix(2).width(8).fill('0')
//				<< (int)lmask[7-iBytes] << sio.nl;
			for( iBytes--; iBytes >= 1; iBytes-- )
			{
//				sio << "iBytes = " << iBytes << ", shift = " << (6*(iBytes-1))
//					<< sio.nl;
//				sio << "next: " << Bu::Fmt().radix(2).width(8).fill('0')
//					<< (int)(uint8_t)*i << sio.nl
//					<< "mask: " << Bu::Fmt().radix(2).width(8).fill('0')
//					<< (int)lmask[6] << sio.nl;
				i++;
				uPt |= ((*i)&lmask[6])<<(6*(iBytes-1));
			}
			sio << uPt;
//			sio << " (" << Bu::Fmt( 8, 2 ).fill('0')
//				<< uPt << ")";
		}
		else
		{
			sio << (int)((uint8_t)*i);
		}
	}
	sio << sio.nl;
}

