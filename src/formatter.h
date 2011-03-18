/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FORMATTER_H
#define BU_FORMATTER_H

#include "bu/string.h"
#include "bu/fmt.h"

namespace Bu
{
	class Stream;

	class Formatter
	{
	public:
		Formatter( Stream &rStream );
		virtual ~Formatter();

		void write( const Bu::String &sStr );
		void write( const void *sStr, int iLen );
		void writeAligned( const Bu::String &sStr );
		void writeAligned( const char *sStr, int iLen );

		void read( void *sStr, int iLen );
		Bu::String readToken();

		void incIndent();
		void decIndent();
		void setIndent( uint8_t uLevel );
		void clearIndent();
		uint8_t getIndent() const { return uIndent; }
		void setIndentChar( char cIndent );
		char getIndentChar() const { return cIndent; }

		void setFormat( const Fmt &f )
		{
			fLast = f;
			bTempFmt = false;
		}

		void setTempFormat( const Fmt &f )
		{
			fLast = f;
			bTempFmt = true;
		}

		void usedFormat()
		{
			if( bTempFmt )
				fLast = Fmt();
		}

		template<typename type>
		void ifmt( type i )
		{
			// This code is taken from Nango, hopefully we can make it better.
			bool bNeg = i<0;
			char cBase = fLast.bCaps?'A':'a';
			char buf[sizeof(type)*8+1];
			if( bNeg ) i = -i;
			if( fLast.uRadix < 2 || fLast.uRadix > 36 )
			{
				usedFormat();
				return;
			}

			for( int j = sizeof(type)*8; j >= 0; j-- )
			{
				int c = i%fLast.uRadix;
				i /= fLast.uRadix;
				buf[j] = (char)((c<10)?('0'+c):(cBase+c-10));
				if( i == 0 )
				{
					if( bNeg ) buf[--j] = '-';
					else if( fLast.bPlus ) buf[--j] = '+';
					writeAligned( buf+j, sizeof(type)*8-j+1 );

					return;
				}
			}
			usedFormat();
		}

		template<typename type>
		void ufmt( type i )
		{
			// This code is taken from Nango, hopefully we can make it better.
			char buf[sizeof(type)*8+1];
			char cBase = fLast.bCaps?'A':'a';
			if( fLast.uRadix < 2 || fLast.uRadix > 36 )
			{
				usedFormat();
				return;
			}

			for( int j = sizeof(type)*8; j >= 0; j-- )
			{
				int c = i%fLast.uRadix;
				i /= fLast.uRadix;
				buf[j] = (char)((c<10)?('0'+c):(cBase+c-10));
				if( i == 0 )
				{
					if( fLast.bPlus ) buf[--j] = '+';
					writeAligned( buf+j, sizeof(type)*8-j+1 );

					return;
				}
			}
			usedFormat();
		}

		template<typename type>
		void ffmt( type f )
		{
			Bu::String fTmp;
//			fTmp.format("%f", f );
//			writeAligned("**make floats work**");
			writeAligned( fTmp );
			usedFormat();
		}
		
		template<typename type>
		void iparse( type &i, const Bu::String &sBuf )
		{
			if( !sBuf.isSet() )
				return;
			if( sBuf[0] != '+' && sBuf[0] != '-' &&
				(sBuf[0] < '0' && sBuf[0] > '9') )
				return;
			int j = 1;
			int iMax = sBuf.getSize();
			for(; j < iMax && (sBuf[j] >= '0' && sBuf[j] <= '9'); j++ ) { }
			i = 0;
			type iPos = 1;
			for(j--; j >= 0; j-- )
			{
				if( sBuf[j] == '+' || sBuf[j] == '-' )
					continue;
				i += (sBuf[j]-'0')*iPos;
				iPos *= fLast.uRadix;
			}
			if( sBuf[0] == '-' )
				i = -i;

			usedFormat();
		}
		
		template<typename type>
		void uparse( type &i, const Bu::String &sBuf )
		{
			if( !sBuf.isSet() )
				return;
			if( sBuf[0] != '+' &&
				(sBuf[0] < '0' && sBuf[0] > '9') )
				return;
			int j = 1;
			int iMax = sBuf.getSize();
			for(; j < iMax && (sBuf[j] >= '0' && sBuf[j] <= '9'); j++ ) { }
			i = 0;
			type iPos = 1;
			for(j--; j >= 0; j-- )
			{
				if( sBuf[j] == '+' )
					continue;
				i += (sBuf[j]-'0')*iPos;
				iPos *= fLast.uRadix;
			}

			usedFormat();
		}
		
		template<typename type>
		void fparse( type &f, const Bu::String &sBuf )
		{
			sscanf( sBuf.getStr(), "%f", &f );
			usedFormat();
		}

		enum Special
		{
			nl,
			flush
		};

		void doFlush();

	private:
		Stream &rStream;
		Fmt fLast;
		bool bTempFmt;
		uint8_t uIndent;
		char cIndent;
	};

	Formatter &operator<<( Formatter &f, const Fmt &fmt );
	Formatter &operator<<( Formatter &f, Formatter::Special s );
	Formatter &operator<<( Formatter &f, const char *sStr );
	Formatter &operator<<( Formatter &f, char *sStr );
	Formatter &operator<<( Formatter &f, const Bu::String &sStr );
	Formatter &operator<<( Formatter &f, signed char c );
	Formatter &operator<<( Formatter &f, char c );
	Formatter &operator<<( Formatter &f, unsigned char c );
	Formatter &operator<<( Formatter &f, signed short i );
	Formatter &operator<<( Formatter &f, unsigned short i );
	Formatter &operator<<( Formatter &f, signed int i );
	Formatter &operator<<( Formatter &f, unsigned int i );
	Formatter &operator<<( Formatter &f, signed long i );
	Formatter &operator<<( Formatter &f, unsigned long i );
	Formatter &operator<<( Formatter &f, signed long long i );
	Formatter &operator<<( Formatter &f, unsigned long long i );
	Formatter &operator<<( Formatter &f, float flt );
	Formatter &operator<<( Formatter &f, double flt );
	Formatter &operator<<( Formatter &f, long double flt );
	Formatter &operator<<( Formatter &f, bool b );

	Formatter &operator>>( Formatter &f, Bu::String &sStr );
	Formatter &operator>>( Formatter &f, signed char &c );
	Formatter &operator>>( Formatter &f, char &c );
	Formatter &operator>>( Formatter &f, unsigned char &c );
	Formatter &operator>>( Formatter &f, signed short &i );
	Formatter &operator>>( Formatter &f, unsigned short &i );
	Formatter &operator>>( Formatter &f, signed int &i );
	Formatter &operator>>( Formatter &f, unsigned int &i );
	Formatter &operator>>( Formatter &f, signed long &i );
	Formatter &operator>>( Formatter &f, unsigned long &i );
	Formatter &operator>>( Formatter &f, signed long long &i );
	Formatter &operator>>( Formatter &f, unsigned long long &i );
	Formatter &operator>>( Formatter &f, float &flt );
	Formatter &operator>>( Formatter &f, double &flt );
	Formatter &operator>>( Formatter &f, long double &flt );
	Formatter &operator>>( Formatter &f, bool &b );

	template<typename type>
	Formatter &operator<<( Formatter &f, const type *p )
	{
		return f << "0x" << Fmt::hex(sizeof(ptrdiff_t)*2) << (ptrdiff_t)(p);
	}
};

#endif
