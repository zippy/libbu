/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/utfstring.h"

#include "bu/string.h"
#include "bu/stream.h"
#include "bu/config.h"
#include "bu/sio.h"
using Bu::sio;

Bu::UtfString::UtfString()
{
}

Bu::UtfString::UtfString( const Bu::String &sInput, Encoding eEnc )
{
	set( sInput, eEnc );
}

Bu::UtfString::~UtfString()
{
}

void Bu::UtfString::set( const Bu::String &sInput, Encoding eEnc )
{
	switch( eEnc )
	{
		case Utf8:
			setUtf8( sInput );
			break;

		case Utf16:
			setUtf16( sInput );
			break;

		case Utf16be:
			setUtf16be( sInput );
			break;

		case Utf16le:
			setUtf16le( sInput );
			break;

		case Utf32:
			setUtf32( sInput );
			break;

		case Utf32be:
			setUtf32be( sInput );
			break;

		case Utf32le:
			setUtf32le( sInput );
			break;

		case Ucs2:
			throw Bu::ExceptionBase("Ucs2 not supported yet.");
			break;

		case Ucs4:
			throw Bu::ExceptionBase("Ucs4 not supported yet.");
			break;

		case GuessEncoding:
			throw Bu::ExceptionBase("Guessing mode not supported yet.");
			break;
	}
}

void Bu::UtfString::append( UtfChar ch )
{
	if( ch >= 0x10000 )
	{
		ch -= 0x10000;
		append16( ((ch>>10)&0x3FF)| 0xD800u );
		append16( (ch&0x3FF)| 0xDC00u );
	}
	else
	{
		append16( (uint16_t)(ch) );
	}
}

void Bu::UtfString::setUtf8( const Bu::String &sInput )
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
	for( Bu::String::const_iterator i = sInput.begin(); i; i++ )
	{
		if( ((int)(uint8_t)*i)&0x80 )
		{
			int iBytes = 1;
			for(; (((uint8_t)(*i))<<iBytes)&0x80; iBytes++ ) { }
			Bu::UtfChar uPt = ((*i) & lmask[7-iBytes])<<(6*(iBytes-1));
			for( iBytes--; iBytes >= 1; iBytes-- )
			{
				i++;
				uPt |= ((*i)&lmask[6])<<(6*(iBytes-1));
			}
			append( uPt );
		}
		else
		{
			append( (Bu::UtfChar)(*i) );
		}
	}
}

void Bu::UtfString::setUtf16( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*sInput.begin() == 0xFF &&
		(uint8_t)*(sInput.begin()+1) == 0xFE )
	{
		setUtf16le( sInput );
		return;
	}
	setUtf16be( sInput );
}

void Bu::UtfString::setUtf16be( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*sInput.begin() == 0xFE &&
		(uint8_t)*(sInput.begin()+1) == 0xFF )

	{
		i += 2;
		sio << "Verified big endian." << sio.nl;
	}
	else
	{
		sio << "Assuming big endian." << sio.nl;
	}
	uint16_t hi, lo;
	for( ; i; i++ )
	{
		hi = (((uint8_t)*i)<<8) | ((uint8_t)*(++i));
		append16( hi );
		if( (hi&0xD800u) == 0xD800u )
		{
			lo = (((uint8_t)*(++i))<<8) | ((uint8_t)*(++i));
			append16( lo );
		}
	}
}

void Bu::UtfString::setUtf16le( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*sInput.begin() == 0xFF &&
		(uint8_t)*(sInput.begin()+1) == 0xFE )
	{
		i += 2;
		sio << "Verified little endian." << sio.nl;
	}
	else
	{
		sio << "Assuming little endian." << sio.nl;
	}
	uint16_t hi, lo;
	for( ; i; i++ )
	{
		hi = (((uint8_t)*i)) | ((uint8_t)*(++i)<<8);
		append16( hi );
		if( (hi&0xD800u) == 0xD800u )
		{
			lo = (((uint8_t)*(++i))) | ((uint8_t)*(++i)<<8);
			append16( lo );
		}
	}
}

void Bu::UtfString::setUtf32( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*i == 0x00 &&
		(uint8_t)*(++i) == 0x00 &&
		(uint8_t)*(++i) == 0xFF &&
		(uint8_t)*(++i) == 0xFE )
	{
		setUtf32le( sInput );
		return;
	}
	setUtf32be( sInput );
}

void Bu::UtfString::setUtf32be( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*i == 0x00 &&
		(uint8_t)*(++i) == 0x00 &&
		(uint8_t)*(++i) == 0xFE &&
		(uint8_t)*(++i) == 0xFF )
	{
		i++;
		sio << "Verified big endian." << sio.nl;
	}
	else
	{
		i = sInput.begin();
		sio << "Assuming big endian." << sio.nl;
	}
	for( ; i; i++ )
	{
		append( (((uint8_t)*i)<<24) |
				(((uint8_t)*(++i))<<16) |
				(((uint8_t)*(++i))<<8) |
				((uint8_t)*(++i))
			  );
	}
}

void Bu::UtfString::setUtf32le( const Bu::String &sInput )
{
	Bu::String::const_iterator i = sInput.begin();
	if( (uint8_t)*i == 0x00 &&
		(uint8_t)*(++i) == 0x00 &&
		(uint8_t)*(++i) == 0xFF &&
		(uint8_t)*(++i) == 0xFE )
	{
		i++;
		sio << "Verified little endian." << sio.nl;
	}
	else
	{
		i = sInput.begin();
		sio << "Assuming little endian." << sio.nl;
	}
	for( ; i; i++ )
	{
		append( ((uint8_t)*i) |
				(((uint8_t)*(++i))<<8) |
				(((uint8_t)*(++i))<<16) |
				(((uint8_t)*(++i))<<24)
			  );
	}
}

void Bu::UtfString::write( Bu::Stream &sOut, Encoding eEnc )
{
	switch( eEnc )
	{
		case Utf8:
			writeUtf8( sOut );
			break;

		case Utf16:
			writeUtf16( sOut );
			break;

		case Utf16be:
			writeUtf16be( sOut );
			break;

		case Utf16le:
			writeUtf16le( sOut );
			break;

		case Utf32:
			writeUtf32( sOut );
			break;

		case Utf32be:
			writeUtf32be( sOut );
			break;

		case Utf32le:
			writeUtf32le( sOut );
			break;

		case Ucs2:
			throw Bu::ExceptionBase("Ucs2 not supported yet.");
			break;

		case Ucs4:
			throw Bu::ExceptionBase("Ucs4 not supported yet.");
			break;

		case GuessEncoding:
			throw Bu::ExceptionBase(
				"GuessEncoding is incompatible with encoding.");
			break;

	}
}

void Bu::UtfString::writeUtf8( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf16( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf16be( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf16le( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf32( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf32be( Bu::Stream &sOut )
{
}

void Bu::UtfString::writeUtf32le( Bu::Stream &sOut )
{
}

Bu::UtfChar Bu::UtfString::get( int iIndex )
{
	return nextChar( iIndex );
}

Bu::UtfChar Bu::UtfString::nextChar( int &iIndex )
{
	Bu::UtfChar i = aData[iIndex++];
	switch( i&0xFC00 )
	{
		case 0xD800:
			return (((i&0x3FF)<<10) | ((aData[iIndex++]&0x3FF)))+0x10000;

		case 0xDC00:
			return (((aData[iIndex-2]&0x3FF)<<10) | ((i&0x3FF)))+0x10000;

		default:
			return i;
	}
}

void Bu::UtfString::debug()
{
	sio << "Raw Utf16: ";
	for( int i = 0; i < aData.getSize(); i++ )
	{
		if( i > 0 )
			sio << ", ";
		sio << "0x" << Fmt::hex() << aData[i];
	}
	sio << sio.nl;
	sio << "Code Points: ";
	for( int i = 0; i < aData.getSize(); i++ )
	{
		if( i > 0 )
			sio << ", ";
		sio << "0x" << Fmt::hex() << get( i );
	}
	sio << sio.nl;
}
/*
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
			Bu::UtfChar uPt = ((*i) & lmask[7-iBytes])<<(6*(iBytes-1));
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
*/
