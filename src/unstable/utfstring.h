/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
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

	/**
	 * A unicode string.  This class represents a string of unicode code points.
	 * Every character in unicode can be represented with 21 bits, but we don't
	 * have a datatype that's 24 bits long, so we return all code points as a
	 * 32 bit unsigned value represented by Bu::UtfChar.  However, the UtfString
	 * class, for efficiency purposes doesn't store 32 bit values internally.
	 * It represents all code points in the native utf16 encodeng.  This means
	 * that it may be very difficult to quickly determine the length of a
	 * UtfString in code points.  Unlike many Unicode handling systems, this
	 * one actually works with complete code points.  When using this class you
	 * don't ever have to know about the inner workings of the different
	 * encoding schemes.  All of the data is dealt with as whole code points.
	 *
	 * As an aside, this means that when encoding a UtfString to a Utf16
	 * encoding that matches your archetecture this operation will be very
	 * fast since it will effectively be a raw dump of the internal data
	 * structures.  However, it is highly reccomended that you DO NOT use the
	 * little endian encodings if you can possibly avoid it.  They are not
	 * reccomended by the Unicode Consortium and are mainly supported as a
	 * means of communicating with other systems that encode their data
	 * incorrectly.  That said, whenever UtfString encodes the contained string
	 * it always includes a BOM at the begining (the byte order marker) so that
	 * proper byte order can be easily determined by the program reading the
	 * data.
	 *
	 *@todo Investigate http://www.unicode.org/reports/tr6/ for compression.
	 */
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

		/**
		 * Append a UtfChar (A unicode code point) to the string.  This can be
		 * any valid code point, and is just the value of the code point, no
		 * encoding necessary.
		 */
		void append( UtfChar ch );

		/**
		 * Set the value of the entire string based on the given input and
		 * encoding.  The default encoding is Utf8, which is compatible with
		 * 7-bit ascii, so it's a great choice for setting UtfStrings from
		 * string literals in code.
		 */
		void set( const Bu::String &sInput, Encoding eEnc=Utf8 );

		/**
		 * This encodes the UtfString in the given encoding and outputs it to
		 * the provided stream.  all Utf16 and Utf32 encodings will have the
		 * correct BOM (byte order marker) at the begining.
		 */
		void write( Bu::Stream &sOut, Encoding eEnc=Utf8 );

		/**
		 * This encodes the UtfString in the given encoding and returns it as
		 * a binary Bu::String.  Like write, this also includes the proper BOM
		 * at the begining.
		 */
		Bu::String get( Encoding eEnc=Utf8 );

		void debug();

		/**
		 * This may or may not stick around, given an index, this returns a
		 * codepoint, however there isn't necesarilly a 1:1 ratio between
		 * indexes and code points.
		 */
		UtfChar get( int iIndex );

		/**
		 * This is what to use if you want to iterate through a section of the
		 * UtfString and you want to use a numerical index.  In most cases it
		 * will be much easier to use an iterator, though.  Given an index this
		 * will return the codepoint at that position and increment iIndex an
		 * appropriate amount for it to point to the next code point.
		 */
		UtfChar nextChar( int &iIndex );

	private:
		void append16( uint16_t i ) { aData.append( i ); }

		void setUtf8( const Bu::String &sInput );
		void setUtf16( const Bu::String &sInput );
		void setUtf16be( const Bu::String &sInput );
		void setUtf16le( const Bu::String &sInput );
		void setUtf32( const Bu::String &sInput );
		void setUtf32be( const Bu::String &sInput );
		void setUtf32le( const Bu::String &sInput );
		
		void writeUtf8( Bu::Stream &sOut );
		void writeUtf16be( Bu::Stream &sOut );
		void writeUtf16le( Bu::Stream &sOut );
		void writeUtf32be( Bu::Stream &sOut );
		void writeUtf32le( Bu::Stream &sOut );

	private:
		Bu::Array<uint16_t> aData;
		int iRawLen;
		int iCharLen;
	};
};

#endif
