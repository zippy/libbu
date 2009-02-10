#ifndef BU_FORMATTER_H
#define BU_FORMATTER_H

#include "stream.h"

namespace Bu
{
	class Formatter
	{
	public:
		Formatter( Stream &rOut );
		virtual ~Formatter();

		typedef struct Fmt
		{
			enum Alignment
			{
				Left	= 0,
				Center	= 1,
				Right	= 2
			};
			Fmt() :
				uMinWidth( 0 ),
				uRadix( 10 ),
				uAlign( Right ),
				bPlus( false ),
				bCaps( false )
			{
			}

			Fmt( unsigned int uMinWidth, unsigned int uRadix=10,
					Alignment a=Right, bool bPlus=false ) :
				uMinWidth( uMinWidth ),
				uRadix( uRadix ),
				uAlign( a ),
				bPlus( bPlus ),
				bCaps( false )
			{
			}

			unsigned int uMinWidth : 8;
			unsigned int uRadix : 6;
			unsigned int uAlign : 2;
			unsigned int bPlus : 1;
			unsigned int bCaps : 1;
		} Fmt;

		void write( const Bu::FString &sStr );
		void write( const char *sStr, int iLen );
		void writeAligned( const Bu::FString &sStr );
		void writeAligned( const char *sStr, int iLen );

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
				buf[j] = (char)((c<10)?('0'+c):('A'+c-10));
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
			if( fLast.uRadix < 2 || fLast.uRadix > 36 )
			{
				usedFormat();
				return;
			}

			for( int j = sizeof(type)*8; j >= 0; j-- )
			{
				int c = i%fLast.uRadix;
				i /= fLast.uRadix;
				buf[j] = (char)((c<10)?('0'+c):('A'+c-10));
				if( i == 0 )
				{
					if( fLast.bPlus ) buf[--j] = '+';
					writeAligned( buf+j, sizeof(type)*8-j+1 );

					return;
				}
			}
			usedFormat();
		}

		enum Special
		{
			nl
		};

	private:
		Stream &rOut;
		Fmt fLast;
		bool bTempFmt;
	};

	typedef Formatter::Fmt Fmt;

	Formatter &operator<<( Formatter &rOut, const Formatter::Fmt &f );
	Formatter &operator<<( Formatter &rOut, Formatter::Special s );
	Formatter &operator<<( Formatter &rOut, const char *sStr );
	Formatter &operator<<( Formatter &rOut, const Bu::FString &sStr );
	Formatter &operator<<( Formatter &rOut, signed char c );
	Formatter &operator<<( Formatter &rOut, char c );
	Formatter &operator<<( Formatter &rOut, unsigned char c );
	Formatter &operator<<( Formatter &rOut, signed short i );
	Formatter &operator<<( Formatter &rOut, unsigned short i );
	Formatter &operator<<( Formatter &rOut, signed int i );
	Formatter &operator<<( Formatter &rOut, unsigned int i );
	Formatter &operator<<( Formatter &rOut, signed long i );
	Formatter &operator<<( Formatter &rOut, unsigned long i );
	Formatter &operator<<( Formatter &rOut, signed long long i );
	Formatter &operator<<( Formatter &rOut, unsigned long long i );
};

#endif
