/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/utfstring.h"

#include "bu/string.h"
#include "bu/stream.h"
#include "bu/config.h"
#include "bu/sio.h"
#include "bu/membuf.h"
using Bu::sio;

Bu::UtfString::UtfString()
{
}

Bu::UtfString::UtfString( const Bu::String &sInput, Encoding eEnc )
{
    set( sInput, eEnc );
}

Bu::UtfString::UtfString( const char *sInput, Encoding eEnc )
{
    set( sInput, eEnc );
}

Bu::UtfString::~UtfString()
{
}

Bu::UtfString::iterator Bu::UtfString::begin()
{
    return Bu::UtfString::iterator( this, 0 );
}

Bu::UtfString::const_iterator Bu::UtfString::begin() const
{
    return Bu::UtfString::const_iterator( this, 0 );
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

void Bu::UtfString::append( const UtfString &rSrc )
{
    aData.append( rSrc.aData );
    iRawLen += rSrc.iRawLen;
    iCharLen += rSrc.iCharLen;
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
//  Bu::String::const_iterator i = sInput.begin();
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

void Bu::UtfString::write( Bu::Stream &sOut, Encoding eEnc ) const
{
    switch( eEnc )
    {
        case Utf8:
            writeUtf8( sOut );
            break;

        case Utf16:
//          writeUtf16( sOut );
//          break;

        case Utf16be:
            writeUtf16be( sOut );
            break;

        case Utf16le:
            writeUtf16le( sOut );
            break;

        case Utf32:
//          writeUtf32( sOut );
//          break;

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

void Bu::UtfString::writeUtf8( Bu::Stream &sOut ) const
{
    int iPos = 0;
    while( iPos < aData.getSize() )
    {
        uint8_t uByte;
        Bu::UtfChar chr = nextChar( iPos );
        if( chr >= 0x010000 )
        {
            // Four bytes
            // 111 111111 111111 111111
            uByte = (chr>>18)|0xF0;
            sOut.write( &uByte, 1 );
            uByte = ((chr>>12)&0x3F)|0x80;
            sOut.write( &uByte, 1 );
            uByte = ((chr>>6)&0x3F)|0x80;
            sOut.write( &uByte, 1 );
            uByte = (chr&0x3F)|0x80;
            sOut.write( &uByte, 1 );
        }
        else if( chr >= 0x800 )
        {
            // Three bytes
            // 1111 111111 111111
            uByte = (chr>>12)|0xE0;
            sOut.write( &uByte, 1 );
            uByte = ((chr>>6)&0x3F)|0x80;
            sOut.write( &uByte, 1 );
            uByte = (chr&0x3F)|0x80;
            sOut.write( &uByte, 1 );
        }
        else if( chr >= 0x80 )
        {
            // Two bytes
            // 11111 111111
            uByte = (chr>>6)|0xC0;
            sOut.write( &uByte, 1 );
            uByte = (chr&0x3F)|0x80;
            sOut.write( &uByte, 1 );
        }
        else
        {
            // One byte
            uByte = chr;
            sOut.write( &uByte, 1 );
        }
    }
}
/*
void Bu::UtfString::writeUtf16( Bu::Stream &sOut )
{
}
*/
void Bu::UtfString::writeUtf16be( Bu::Stream &sOut ) const
{
#if BYTE_ORDER == BIG_ENDIAN
    uint16_t iTmp = 0xFEFF; // Byte Order Marker
    sOut.write( &iTmp, 2 );
    for( Array<uint16_t>::const_iterator i = aData.begin(); i; i++ )
    {
        iTmp = *i;
        sOut.write( &iTmp, 2 );
    }
#else
    uint16_t iTmp = 0xFEFF; // Byte Order Marker
    iTmp = (iTmp>>8) | (iTmp<<8);
    sOut.write( &iTmp, 2 );
    for( Array<uint16_t>::const_iterator i = aData.begin(); i; i++ )
    {
        iTmp = *i;
        iTmp = (iTmp>>8) | (iTmp<<8);
        sOut.write( &iTmp, 2 );
    }
#endif
}

void Bu::UtfString::writeUtf16le( Bu::Stream &sOut ) const
{
#if BYTE_ORDER == LITTLE_ENDIAN
    uint16_t iTmp = 0xFEFF; // Byte Order Marker
    sOut.write( &iTmp, 2 );
    for( Array<uint16_t>::const_iterator i = aData.begin(); i; i++ )
    {
        iTmp = *i;
        sOut.write( &iTmp, 2 );
    }
#else
    uint16_t iTmp = 0xFEFF; // Byte Order Marker
    iTmp = (iTmp>>8) | (iTmp<<8);
    sOut.write( &iTmp, 2 );
    for( Array<uint16_t>::const_iterator i = aData.begin(); i; i++ )
    {
        iTmp = *i;
        iTmp = (iTmp>>8) | (iTmp<<8);
        sOut.write( &iTmp, 2 );
    }
#endif
}

void Bu::UtfString::writeUtf32be( Bu::Stream &sOut ) const
{
#if BYTE_ORDER == BIG_ENDIAN
    uint32_t iTmp = 0xFEFF; // Byte Order Marker
    sOut.write( &iTmp, 4 );
    int i = 0;
    while( i < aData.getSize() )
    {
        iTmp = nextChar( i );
        sOut.write( &iTmp, 4 );
    }
#else
    uint32_t iTmp = 0xFEFF; // Byte Order Marker
    iTmp = (iTmp>>24)|(iTmp<<24)|((iTmp&0xff0000)>>8)|((iTmp&0xff00)<<8);
    sOut.write( &iTmp, 4 );
    int i = 0;
    while( i < aData.getSize() )
    {
        iTmp = nextChar( i );
        iTmp = (iTmp>>24)|(iTmp<<24)|((iTmp&0xff0000)>>8)|((iTmp&0xff00)<<8);
        sOut.write( &iTmp, 4 );
    }
#endif
}

void Bu::UtfString::writeUtf32le( Bu::Stream &sOut ) const
{
#if BYTE_ORDER == LITTLE_ENDIAN
    uint32_t iTmp = 0xFEFF; // Byte Order Marker
    sOut.write( &iTmp, 4 );
    int i = 0;
    while( i < aData.getSize() )
    {
        iTmp = nextChar( i );
        sOut.write( &iTmp, 4 );
    }
#else
    uint32_t iTmp = 0xFEFF; // Byte Order Marker
    iTmp = (iTmp>>24)|(iTmp<<24)|((iTmp&0xff0000)>>8)|((iTmp&0xff00)<<8);
    sOut.write( &iTmp, 4 );
    int i = 0;
    while( i < aData.getSize() )
    {
        iTmp = nextChar( i );
        iTmp = (iTmp>>24)|(iTmp<<24)|((iTmp&0xff0000)>>8)|((iTmp&0xff00)<<8);
        sOut.write( &iTmp, 4 );
    }
#endif
}

Bu::UtfChar Bu::UtfString::get( int iIndex ) const
{
    return nextChar( iIndex );
}

Bu::UtfChar Bu::UtfString::nextChar( int &iIndex ) const
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

bool Bu::UtfString::operator==( const Bu::UtfString &rhs ) const
{
    return aData == rhs.aData;
}

Bu::UtfString &Bu::UtfString::operator+=( const Bu::UtfString &rhs )
{
    append( rhs );
    return *this;
}

Bu::UtfString &Bu::UtfString::operator+=( const UtfChar &rhs )
{
    append( rhs );
    return *this;
}

Bu::String Bu::UtfString::get( Encoding eEnc ) const
{
    Bu::MemBuf mb;
    write( mb, eEnc );
    return mb.getString();
}

void Bu::UtfString::debug() const
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
        sio << "0x" << Fmt::hex() << nextChar( i );
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
//          sio << "Flag byte: " << Bu::Fmt().radix(2).width(8).fill('0')
//              << (int)(uint8_t)*i << sio.nl;
            int iBytes = 1;
            for(; (((uint8_t)(*i))<<iBytes)&0x80; iBytes++ ) { }
//          sio << "iBytes = " << iBytes << sio.nl;
            Bu::UtfChar uPt = ((*i) & lmask[7-iBytes])<<(6*(iBytes-1));
//          sio << "mask: " << Bu::Fmt().radix(2).width(8).fill('0')
//              << (int)lmask[7-iBytes] << sio.nl;
            for( iBytes--; iBytes >= 1; iBytes-- )
            {
//              sio << "iBytes = " << iBytes << ", shift = " << (6*(iBytes-1))
//                  << sio.nl;
//              sio << "next: " << Bu::Fmt().radix(2).width(8).fill('0')
//                  << (int)(uint8_t)*i << sio.nl
//                  << "mask: " << Bu::Fmt().radix(2).width(8).fill('0')
//                  << (int)lmask[6] << sio.nl;
                i++;
                uPt |= ((*i)&lmask[6])<<(6*(iBytes-1));
            }
            sio << uPt;
//          sio << " (" << Bu::Fmt( 8, 2 ).fill('0')
//              << uPt << ")";
        }
        else
        {
            sio << (int)((uint8_t)*i);
        }
    }
    sio << sio.nl;
}
*/

template<> uint32_t Bu::__calcHashCode<Bu::UtfString>( const Bu::UtfString &k )
{
    uint32_t uCode = 0;

    for( Bu::UtfString::const_iterator i = k.begin(); i; i++ )
    {
        uCode = *i + (uCode<<6) + (uCode<<16) - uCode;
    }

    return uCode;
}

template<> bool Bu::__cmpHashKeys<Bu::UtfString>(
        const Bu::UtfString &a, const Bu::UtfString &b )
{
    return a == b;
}
