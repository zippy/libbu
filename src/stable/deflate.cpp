/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/deflate.h"
#include "bu/trace.h"

#include <zlib.h>

#define pState ((z_stream *)prState)

using namespace Bu;

Bu::Deflate::Deflate( Bu::Stream &rNext, int nCompression, Format eFmt ) :
    Bu::Filter( rNext ),
    prState( NULL ),
    nCompression( nCompression ),
    sTotalOut( 0 ),
    eFmt( eFmt ),
    bEos( false )
{
    TRACE( nCompression );
    start();
}

Bu::Deflate::~Deflate()
{
    TRACE();
    stop();
}

void Bu::Deflate::start()
{
    TRACE();
    prState = new z_stream;
    pState->zalloc = NULL;
    pState->zfree = NULL;
    pState->opaque = NULL;
    pState->state = NULL;

    nBufSize = 64*1024;
    pBuf = new char[nBufSize];
}

Bu::size Bu::Deflate::stop()
{
    TRACE();
    if( pState && pState->state )
    {
        if( bReading )
        {
            inflateEnd( pState );
            delete[] pBuf;
            pBuf = NULL;
            delete pState;
            prState = NULL;
            return 0;
        }
        else
        {
            for(;;)
            {
                pState->next_in = NULL;
                pState->avail_in = 0;
                pState->avail_out = nBufSize;
                pState->next_out = (Bytef *)pBuf;
                int res = deflate( pState, Z_FINISH );
                if( pState->avail_out < nBufSize )
                {
                    sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
                }
                if( res == Z_STREAM_END )
                    break;
            }
            deflateEnd( pState );
            delete[] pBuf;
            pBuf = NULL;
            delete pState;
            prState = NULL;
            return sTotalOut;
        }
    }
    return 0;
}

void Bu::Deflate::zError( int code )
{
    TRACE( code );
    switch( code )
    {
        case Z_OK:
        case Z_STREAM_END:
        case Z_NEED_DICT:
            return;

        case Z_ERRNO:
            throw ExceptionBase("Deflate: Errno - %s", pState->msg );

        case Z_STREAM_ERROR:
            throw ExceptionBase("Deflate: Stream Error - %s", pState->msg );

        case Z_DATA_ERROR:
            throw ExceptionBase("Deflate: Data Error - %s", pState->msg );

        case Z_MEM_ERROR:
            throw ExceptionBase("Deflate: Mem Error - %s", pState->msg );

        case Z_BUF_ERROR:
            throw ExceptionBase("Deflate: Buf Error - %s", pState->msg );

        case Z_VERSION_ERROR:
            throw ExceptionBase("Deflate: Version Error - %s", pState->msg );

        default:
            throw ExceptionBase("Deflate: Unknown error encountered - %s.", pState->msg );
    
    }
}

Bu::size Bu::Deflate::read( void *pData, Bu::size nBytes )
{
    TRACE( pData, nBytes );
    if( nBytes  <= 0 )
        return 0;
    if( !pState->state )
    {
        bReading = true;
        if( eFmt&AutoDetect )
            inflateInit2( pState, 32+15 ); // Auto-detect, large window
        else if( eFmt == Raw )
            inflateInit2( pState, -15 ); // Raw
        else if( eFmt == Zlib )
            inflateInit2( pState, 15 ); // Zlib
        else if( eFmt == Gzip )
            inflateInit2( pState, 16+15 ); // GZip
        else
            throw Bu::ExceptionBase("Format mode for deflate read.");
        pState->next_in = (Bytef *)pBuf;
        pState->avail_in = 0;
    }
    if( bReading == false )
        throw ExceptionBase("This deflate filter is in writing mode, you can't read.");

    int nRead = 0;
    int nReadTotal = pState->total_out;
    pState->next_out = (Bytef *)pData;
    pState->avail_out = nBytes;
    for(;;)
    {
        int ret = inflate( pState, Z_NO_FLUSH );
        nReadTotal += nRead-pState->avail_out;

        if( ret == Z_STREAM_END )
        {
            bEos = true;
            if( pState->avail_in > 0 )
            {
                if( rNext.isSeekable() )
                {
                    rNext.seek( -pState->avail_in );
                }
            }
            return nBytes-pState->avail_out;
        }
        if( ret != Z_BUF_ERROR )
            zError( ret );

        if( pState->avail_out )
        {
            if( pState->avail_in == 0 )
            {
                nRead = rNext.read( pBuf, nBufSize );
                if( nRead == 0 && rNext.isEos() )
                {
                    throw Bu::ExceptionBase("Premature end of underlying "
                            "stream found reading deflate stream.");
                }
                pState->next_in = (Bytef *)pBuf;
                pState->avail_in = nRead;
            }
        }
        else
        {
            return nBytes-pState->avail_out;
        }
    }
    return 0;
}

Bu::size Bu::Deflate::write( const void *pData, Bu::size nBytes )
{
    TRACE( pData, nBytes );
    if( nBytes  <= 0 )
        return 0;
    if( !pState->state )
    {
        bReading = false;
        int iFmt = eFmt&Gzip;
        if( iFmt == Raw )
            deflateInit2( pState, nCompression, Z_DEFLATED, -15, 9,
                Z_DEFAULT_STRATEGY );
        else if( iFmt == Zlib )
            deflateInit2( pState, nCompression, Z_DEFLATED, 15, 9,
                Z_DEFAULT_STRATEGY );
        else if( iFmt == Gzip )
            deflateInit2( pState, nCompression, Z_DEFLATED, 16+15, 9,
                Z_DEFAULT_STRATEGY );
        else
            throw Bu::ExceptionBase("Invalid format for deflate.");
    }
    if( bReading == true )
        throw ExceptionBase("This deflate filter is in reading mode, you can't write.");

    pState->next_in = (Bytef *)pData;
    pState->avail_in = nBytes;
    for(;;)
    {
        pState->avail_out = nBufSize;
        pState->next_out = (Bytef *)pBuf;

        zError( deflate( pState, Z_NO_FLUSH ) );

        if( pState->avail_out < nBufSize )
        {
            sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
        }
        if( pState->avail_in == 0 )
            break;
    }

    return nBytes;
}

bool Bu::Deflate::isOpen()
{
    TRACE();
    return (pState != NULL && pState->state != NULL);
}

bool Bu::Deflate::isEos()
{
    TRACE();
    return bEos;
}

Bu::size Bu::Deflate::getCompressedSize()
{
    return sTotalOut;
}

