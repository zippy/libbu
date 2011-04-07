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
	class Stream;

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
			Utf32be,
			Utf32le,
			Ucs2,
			Ucs4,
			GuessEncoding
		};

		UtfString();
		UtfString( const Bu::String &sInput, Encoding eEnc=Utf8 );
		virtual ~UtfString();

		class iterator
		{
		private:
			iterator( UtfString *pSrc, int iCodePos ) :
				pSrc( pSrc ), iCodePos( iCodePos )
			{
			}

		public:
			iterator() :
				pSrc( NULL ), iCodePos( 0 )
			{
			}

			UtfChar operator*()
			{
				if( !pSrc )
					throw Bu::ExceptionBase("invalid UtfString::iterator dereferenced.");
				return pSrc->nextChar( iCodePos );
			}

		private:
			UtfString *pSrc;
			int iCodePos;
		};

		void append( UtfChar ch );

		void set( const Bu::String &sInput, Encoding eEnc=Utf8 );
		void setUtf8( const Bu::String &sInput );
		void setUtf16( const Bu::String &sInput );
		void setUtf16be( const Bu::String &sInput );
		void setUtf16le( const Bu::String &sInput );
		void setUtf32( const Bu::String &sInput );
		void setUtf32be( const Bu::String &sInput );
		void setUtf32le( const Bu::String &sInput );

		void write( Bu::Stream &sOut, Encoding eEnc=Utf8 );
		void writeUtf8( Bu::Stream &sOut );
		void writeUtf16be( Bu::Stream &sOut );
		void writeUtf16le( Bu::Stream &sOut );
		void writeUtf32be( Bu::Stream &sOut );
		void writeUtf32le( Bu::Stream &sOut );

		Bu::String to( Encoding eEnc=Utf8 );
		Bu::String toUtf8();

		void debug();

		UtfChar get( int iIndex );
		UtfChar nextChar( int &iIndex );

	private:
		void append16( uint16_t i ) { aData.append( i ); }

	private:
		Bu::Array<uint16_t> aData;
		int iRawLen;
		int iCharLen;
	};
};

#endif
