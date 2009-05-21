#include "formatter.h"

Bu::Formatter::Formatter( Stream &rOut ) :
	rOut( rOut ),
	uIndent( 0 ),
	cIndent( '\t' )
{
}

Bu::Formatter::~Formatter()
{
}

void Bu::Formatter::write( const Bu::FString &sStr )
{
	rOut.write( sStr );
}

void Bu::Formatter::write( const void *sStr, int iLen )
{
	rOut.write( sStr, iLen );
}

void Bu::Formatter::writeAligned( const Bu::FString &sStr )
{
	int iLen = sStr.getSize();
	if( iLen > fLast.uMinWidth )
	{
		write( sStr );
	}
	else
	{
		int iRem = fLast.uMinWidth - iLen;
		switch( fLast.uAlign )
		{
			case Fmt::Right:
				for( int k = 0; k < iRem; k++ )
					write( &fLast.cFillChar, 1 );
				write( sStr );
				break;
			
			case Fmt::Center:
				{
					int iHlf = iRem/2;
					for( int k = 0; k < iHlf; k++ )
						write( &fLast.cFillChar, 1 );
					write( sStr );
					iHlf = iRem-iHlf;;
					for( int k = 0; k < iHlf; k++ )
						write( &fLast.cFillChar, 1 );
				}
				break;
			
			case Fmt::Left:
				write( sStr );
				for( int k = 0; k < iRem; k++ )
					write( &fLast.cFillChar, 1 );
				break;
		}
	}

	usedFormat();
}

void Bu::Formatter::writeAligned( const char *sStr, int iLen )
{
	if( iLen > fLast.uMinWidth )
	{
		write( sStr, iLen );
	}
	else
	{
		int iRem = fLast.uMinWidth - iLen;
		switch( fLast.uAlign )
		{
			case Fmt::Right:
				for( int k = 0; k < iRem; k++ )
					write( &fLast.cFillChar, 1 );
				write( sStr, iLen );
				break;
			
			case Fmt::Center:
				{
					int iHlf = iRem/2;
					for( int k = 0; k < iHlf; k++ )
						write( &fLast.cFillChar, 1 );
					write( sStr, iLen );
					iHlf = iRem-iHlf;;
					for( int k = 0; k < iHlf; k++ )
						write( &fLast.cFillChar, 1 );
				}
				break;
			
			case Fmt::Left:
				write( sStr, iLen );
				for( int k = 0; k < iRem; k++ )
					write( &fLast.cFillChar, 1 );
				break;
		}
	}

	usedFormat();
}

void Bu::Formatter::incIndent()
{
	if( uIndent < 0xFFU )
		uIndent++;
}

void Bu::Formatter::decIndent()
{
	if( uIndent > 0 )
		uIndent--;
}

void Bu::Formatter::setIndent( uint8_t uLevel )
{
	uIndent = uLevel;
}

void Bu::Formatter::clearIndent()
{
	uIndent = 0;
}

void Bu::Formatter::setIndentChar( char cIndent )
{
	this->cIndent = cIndent;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::width( unsigned int uWidth )
{
	this->uMinWidth = uWidth;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::fill( char cFill )
{
	this->cFillChar = (unsigned char)cFill;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::radix( unsigned int uRadix )
{
	this->uRadix = uRadix;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::align( Alignment eAlign )
{
	this->uAlign = eAlign;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::left()
{
	this->uAlign = Fmt::Left;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::center()
{
	this->uAlign = Fmt::Center;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::right()
{
	this->uAlign = Fmt::Right;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::plus( bool bPlus )
{
	this->bPlus = bPlus;
	return *this;
}

Bu::Formatter::Fmt &Bu::Formatter::Fmt::caps( bool bCaps )
{
	this->bCaps = bCaps;
	return *this;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, const Bu::Formatter::Fmt &f )
{
	rOut.setTempFormat( f );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, Bu::Formatter::Special s )
{
	switch( s )
	{
		case Formatter::nl:
			{
				rOut.write("\n", 1 );
				char ci = rOut.getIndentChar();
				for( int j = 0; j < rOut.getIndent(); j++ )
					rOut.write( &ci, 1 );
			}
			break;

		case Formatter::flush:
			rOut.doFlush();
			break;
	}
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, const char *sStr )
{
	rOut.writeAligned( sStr, strlen( sStr ) );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, char *sStr )
{
	rOut.writeAligned( sStr, strlen( sStr ) );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, const Bu::FString &sStr )
{
	rOut.writeAligned( sStr );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, signed char c )
{
	rOut.write( (char *)&c, 1 );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, char c )
{
	rOut.write( (char *)&c, 1 );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, unsigned char c )
{
	rOut.write( (char *)&c, 1 );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, signed short i )
{
	rOut.ifmt<signed short>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, unsigned short i )
{
	rOut.ufmt<unsigned short>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, signed int i )
{
	rOut.ifmt<signed int>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, unsigned int i )
{
	rOut.ufmt<unsigned int>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, signed long i )
{
	rOut.ifmt<signed long>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, unsigned long i )
{
	rOut.ufmt<unsigned long>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, signed long long i )
{
	rOut.ifmt<signed long long>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, unsigned long long i )
{
	rOut.ufmt<unsigned long long>( i );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, float f )
{
	rOut.ffmt<float>( f );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, double f )
{
	rOut.ffmt<double>( f );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, long double f )
{
	rOut.ffmt<long double>( f );
	return rOut;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &rOut, bool b )
{
	rOut.writeAligned( b?("true"):("false") );
	return rOut;
}

