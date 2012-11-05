/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/formatter.h"

#include "bu/stream.h"
#include <string.h>

template<> float Bu::tlog( float x )
{
    return logf( x );
}

template<> double Bu::tlog( double x )
{
    return log( x );
}

template<> long double Bu::tlog( long double x )
{
    return logl( x );
}

template<> float Bu::tfloor( float x )
{
    return floorf( x );
}

template<> double Bu::tfloor( double x )
{
    return floor( x );
}

template<> long double Bu::tfloor( long double x )
{
    return floorl( x );
}

template<> float Bu::tpow( float x, float y )
{
    return powf( x, y );
}

template<> double Bu::tpow( double x, double y )
{
    return pow( x, y );
}

template<> long double Bu::tpow( long double x, long double y )
{
    return powl( x, y );
}

Bu::Formatter::Formatter( Stream &rStream ) :
    rStream( rStream ),
    bTempFmt( false ),
    uIndent( 0 ),
    cIndent( '\t' )
{
}

Bu::Formatter::~Formatter()
{
}

void Bu::Formatter::write( const Bu::String &sStr )
{
    rStream.write( sStr );
}

void Bu::Formatter::write( const void *sStr, int iLen )
{
    rStream.write( sStr, iLen );
}

void Bu::Formatter::writeAligned( const Bu::String &sStr )
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

void Bu::Formatter::read( void *sStr, int iLen )
{
    rStream.read( sStr, iLen );
}

Bu::String Bu::Formatter::readToken()
{
    Bu::String sRet;
    if( fLast.bTokenize )
    {
        for(;;)
        {
            char buf;
            int iRead = rStream.read( &buf, 1 );
            if( iRead == 0 )
                return sRet;
            if( buf == ' ' || buf == '\t' || buf == '\n' || buf == '\r' )
                continue;
            else
            {
                sRet += buf;
                break;
            }
        }
        for(;;)
        {
            char buf;
            int iRead = rStream.read( &buf, 1 );
            if( iRead == 0 )
                return sRet;
            if( buf == ' ' || buf == '\t' || buf == '\n' || buf == '\r' )
                return sRet;
            else
                sRet += buf;
        }
    }
    else
    {
        for(;;)
        {
            char buf;
            int iRead = rStream.read( &buf, 1 );
            if( iRead == 0 )
                return sRet;
            else
                sRet += buf;
        }
    }
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

void Bu::Formatter::doFlush()
{
    rStream.flush();
}

Bu::Fmt &Bu::Fmt::width( unsigned int uWidth )
{
    this->uMinWidth = uWidth;
    return *this;
}

Bu::Fmt &Bu::Fmt::fill( char cFill )
{
    this->cFillChar = (unsigned char)cFill;
    return *this;
}

Bu::Fmt &Bu::Fmt::radix( unsigned int uRadix )
{
    this->uRadix = uRadix;
    return *this;
}

Bu::Fmt &Bu::Fmt::align( Alignment eAlign )
{
    this->uAlign = eAlign;
    return *this;
}

Bu::Fmt &Bu::Fmt::left()
{
    this->uAlign = Fmt::Left;
    return *this;
}

Bu::Fmt &Bu::Fmt::center()
{
    this->uAlign = Fmt::Center;
    return *this;
}

Bu::Fmt &Bu::Fmt::right()
{
    this->uAlign = Fmt::Right;
    return *this;
}

Bu::Fmt &Bu::Fmt::plus( bool bPlus )
{
    this->bPlus = bPlus;
    return *this;
}

Bu::Fmt &Bu::Fmt::caps( bool bCaps )
{
    this->bCaps = bCaps;
    return *this;
}

Bu::Fmt &Bu::Fmt::upper()
{
    this->bCaps = true;
    return *this;
}

Bu::Fmt &Bu::Fmt::lower()
{
    this->bCaps = false;
    return *this;
}

Bu::Fmt &Bu::Fmt::tokenize( bool bTokenize )
{
    this->bTokenize = bTokenize;
    return *this;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::Fmt &fmt )
{
    f.setTempFormat( fmt );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, Bu::Formatter::Special s )
{
    switch( s )
    {
        case Formatter::nl:
            {
#ifdef WIN32
                f.write("\r\n", 2 );
#else
                f.write("\n", 1 );
#endif
                char ci = f.getIndentChar();
                for( int j = 0; j < f.getIndent(); j++ )
                    f.write( &ci, 1 );
                f.doFlush();
            }
            break;

        case Formatter::flush:
            f.doFlush();
            break;
    }
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const char *sStr )
{
    f.writeAligned( sStr, strlen( sStr ) );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, char *sStr )
{
    f.writeAligned( sStr, strlen( sStr ) );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::String &sStr )
{
    f.writeAligned( sStr );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, signed char c )
{
    f.ifmt<signed char>( c );
    //f.write( (char *)&c, 1 );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, char c )
{
    f.write( (char *)&c, 1 );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, unsigned char c )
{
    f.ufmt<unsigned char>( c );
    //f.write( (char *)&c, 1 );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, signed short i )
{
    f.ifmt<signed short>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, unsigned short i )
{
    f.ufmt<unsigned short>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, signed int i )
{
    f.ifmt<signed int>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, unsigned int i )
{
    f.ufmt<unsigned int>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, signed long i )
{
    f.ifmt<signed long>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, unsigned long i )
{
    f.ufmt<unsigned long>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, signed long long i )
{
    f.ifmt<signed long long>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, unsigned long long i )
{
    f.ufmt<unsigned long long>( i );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, float flt )
{
    f.ffmt<float>( flt );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, double flt )
{
    f.ffmt<double>( flt );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, long double flt )
{
    f.ffmt<long double>( flt );
    return f;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, bool b )
{
    f.writeAligned( b?("true"):("false") );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, Bu::String &sStr )
{
    sStr = f.readToken();
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, signed char &c )
{
    f.read( &c, 1 );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, char &c )
{
    f.read( &c, 1 );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, unsigned char &c )
{
    f.read( &c, 1 );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, signed short &i )
{
    f.iparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, unsigned short &i )
{
    f.uparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, signed int &i )
{
    f.iparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, unsigned int &i )
{
    f.uparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, signed long &i )
{
    f.iparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, unsigned long &i )
{
    f.uparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, signed long long &i )
{
    f.iparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, unsigned long long &i )
{
    f.uparse( i, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, float &flt )
{
    f.fparse( flt, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, double &flt )
{
    f.fparse( flt, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, long double &flt )
{
    f.fparse( flt, f.readToken() );
    return f;
}

Bu::Formatter &Bu::operator>>( Bu::Formatter &f, bool &b )
{
    Bu::String sStr = f.readToken();
    if( !sStr.isSet() )
        return f;
    char c = *sStr.begin();
    if( c == 'y' || c == 'Y' || c == 't' || c == 'T' )
        b = true;
    else if( c == 'n' || c == 'N' || c == 'f' || c == 'F' )
        b = false;

    return f;
}

