/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UTF_STRING_H
#define BU_UTF_STRING_H

#include <stdint.h>
#include "bu/array.h"

namespace Bu
{
	class String;

	/**
	 * UtfChar isn't actually a character, unicode specifies "code points" not
	 * characters.  The main reason for this is that not all code points define
	 * usable characters.  Some control text directionality, some apply
	 * properties to other code points which are characters.  However, most of
	 * these distinctions are only important when implementing displays that
	 * comply with the Unicode standard fully.
	 */
	typedef uint32_t UtfChar;

	class UtfString
	{
	public:
		enum Encoding
		{
			Utf8,
			Utf16,
			Utf16be,
			Utf16le,
			Utf32,
			Ucs16,
			GuessEncoding
		};

		UtfString();
		UtfString( const Bu::String &sInput, Encoding eEnc=Utf8 );
		virtual ~UtfString();

		void append( UtfChar ch );

		void set( const Bu::String &sInput, Encoding eEnc=Utf8 );
		void setUtf8( const Bu::String &sInput );
		void setUtf16( const Bu::String &sInput );
//		void setUtf16be( const Bu::String &sInput );
//		void setUtf16le( const Bu::String &sInput );

		void debug();

		UtfChar get( int iIndex );

	private:
		void append16( uint16_t i ) { aData.append( i ); }

	private:
		Bu::Array<uint16_t> aData;
		int iRawLen;
		int iCharLen;
	};
};

#endif
