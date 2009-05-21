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
				cFillChar(' '),
				uRadix( 10 ),
				uAlign( Right ),
				bPlus( false ),
				bCaps( true )
			{
			}

			Fmt( unsigned int uMinWidth, unsigned int uRadix=10,
					Alignment a=Right, bool bPlus=false, bool bCaps=true,
					char cFill=' ') :
				uMinWidth( uMinWidth ),
				cFillChar(cFill),
				uRadix( uRadix ),
				uAlign( a ),
				bPlus( bPlus ),
				bCaps( bCaps )
			{
			}
			Fmt( unsigned int uMinWidth, Alignment a,
					unsigned int uRadix=10, bool bPlus=false, bool bCaps=true,
					char cFill=' ') :
				uMinWidth( uMinWidth ),
				cFillChar(cFill),
				uRadix( uRadix ),
				uAlign( a ),
				bPlus( bPlus ),
				bCaps( bCaps )
			{
			}

			static Fmt hex( unsigned int uWidth=0, bool bCaps=true )
			{
				return Fmt( uWidth, 16, Right, false, bCaps, '0' );
			}
			
			static Fmt oct( unsigned int uWidth=0, bool bCaps=true )
			{
				return Fmt( uWidth, 8, Right, false, bCaps, '0' );
			}

			static Fmt ptr( bool bCaps=true )
			{
				return Fmt( sizeof(ptrdiff_t)*2, 16, Right, false, bCaps, '0' );
			}

			Fmt &width( unsigned int uWidth );
			Fmt &fill( char cFill='0' );
			Fmt &radix( unsigned int uRadix );
			Fmt &align( Alignment eAlign );
			Fmt &plus( bool bPlus=true );
			Fmt &caps( bool bCaps=true );

			Fmt &left();
			Fmt &right();
			Fmt &center();

			unsigned char uMinWidth;
			char cFillChar;
			unsigned short uRadix : 6;
			unsigned short uAlign : 2;
			unsigned short bPlus : 1;
			unsigned short bCaps : 1;
		} Fmt;

		void write( const Bu::FString &sStr );
		void write( const void *sStr, int iLen );
		void writeAligned( const Bu::FString &sStr );
		void writeAligned( const char *sStr, int iLen );

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

		template<typename type>
		void ffmt( type f )
		{
			Bu::FString fTmp;
			fTmp.format("%f {~!~}", f );
//			writeAligned("**make floats work**");
			writeAligned( fTmp );
			usedFormat();
		}

		enum Special
		{
			nl,
			flush
		};

		void doFlush()
		{
			rOut.flush();
		}

	private:
		Stream &rOut;
		Fmt fLast;
		bool bTempFmt;
		uint8_t uIndent;
		char cIndent;
	};

	typedef Formatter::Fmt Fmt;

	Formatter &operator<<( Formatter &rOut, const Formatter::Fmt &f );
	Formatter &operator<<( Formatter &rOut, Formatter::Special s );
	Formatter &operator<<( Formatter &rOut, const char *sStr );
	Formatter &operator<<( Formatter &rOut, char *sStr );
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
	Formatter &operator<<( Formatter &rOut, float f );
	Formatter &operator<<( Formatter &rOut, double f );
	Formatter &operator<<( Formatter &rOut, long double f );
	Formatter &operator<<( Formatter &rOut, bool b );

	template<typename type>
	Formatter &operator<<( Formatter &rOut, type *p )
	{
		rOut << (ptrdiff_t)(p);
		return rOut;
	}
};

#endif
