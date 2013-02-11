/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/lzma.h"
#include "bu/trace.h"

#include <lzma.h>

#define pState ((lzma_stream *)prState)

using namespace Bu;

Bu::Lzma::Lzma( Bu::Stream &rNext, int nCompression, Format eFmt ) :
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

Bu::Lzma::~Lzma()
{
    TRACE();
    stop();
}

void Bu::Lzma::start()
{
    TRACE();
    nBufSize = 64*1024;
    pBuf = new char[nBufSize];
}

Bu::size Bu::Lzma::stop()
{
    TRACE();
    if( pState )
    {
        if( bReading )
        {
            lzma_end( pState );
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
                pState->next_out = (uint8_t *)pBuf;
                int res = lzma_code( pState, LZMA_FINISH );
                if( pState->avail_out < nBufSize )
                {
                    sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
                }
                if( res == LZMA_STREAM_END )
                    break;
            }
            lzma_end( pState );
            delete[] pBuf;
            pBuf = NULL;
            delete pState;
            prState = NULL;
            return sTotalOut;
        }
    }
    return 0;
}

void Bu::Lzma::lzmaError( int code )
{
    TRACE( code );
    switch( code )
    {
        case LZMA_OK:
        case LZMA_STREAM_END:
        case LZMA_NO_CHECK:
        case LZMA_UNSUPPORTED_CHECK:
            break;

        case LZMA_MEM_ERROR:
            throw ExceptionBase("Lzma: Memory allocation error.");

        case LZMA_MEMLIMIT_ERROR:
            throw ExceptionBase("Lzma: Memory usage limit was reached.");

        case LZMA_FORMAT_ERROR:
            throw ExceptionBase("Lzma: File format not recognized.");

        case LZMA_OPTIONS_ERROR:
            throw ExceptionBase("Lzma: Invalid or unsupported options.");

        case LZMA_DATA_ERROR:
            throw ExceptionBase("Lzma: Data is corrupt.");

        case LZMA_BUF_ERROR:
            throw ExceptionBase("Lzma: No progress is possible.");

        case LZMA_PROG_ERROR:
            throw ExceptionBase("Lzma: Programming error.");

        default:
            throw ExceptionBase("Lzma: Unknown error encountered." );
    }
}

Bu::size Bu::Lzma::read( void *pData, Bu::size nBytes )
{
    TRACE( pData, nBytes );
    if( !pState )
    {
        prState = new ::lzma_stream;
        lzma_stream zEmpty = LZMA_STREAM_INIT;
        Bu::memcpy( prState, &zEmpty, sizeof(lzma_stream) );

        bReading = true;
        lzmaError( lzma_auto_decoder( pState, UINT64_MAX, 0 ) );
        pState->next_in = (uint8_t *)pBuf;
        pState->avail_in = 0;
    }
    if( bReading == false )
        throw ExceptionBase("This lzma filter is in writing mode, you can't read.");

    int nRead = 0;
    int nReadTotal = pState->total_out;
    pState->next_out = (uint8_t *)pData;
    pState->avail_out = nBytes;
    for(;;)
    {
        int ret = lzma_code( pState, LZMA_RUN );
//      printf("inflate returned %d; avail in=%d, out=%d\n", ret,
//              pState->avail_in, pState->avail_out );
    
        nReadTotal += nRead-pState->avail_out;

        if( ret == LZMA_STREAM_END )
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
//      if( ret != LZMA_BUF_ERROR )
            lzmaError( ret );

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
                pState->next_in = (uint8_t *)pBuf;
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

Bu::size Bu::Lzma::write( const void *pData, Bu::size nBytes )
{
    TRACE( pData, nBytes );
    if( !pState )
    {
        prState = new ::lzma_stream;
        lzma_stream zEmpty = LZMA_STREAM_INIT;
        Bu::memcpy( prState, &zEmpty, sizeof(lzma_stream) );

        bReading = false;
        if( eFmt == Xz )
            lzmaError(
                lzma_easy_encoder( pState, nCompression, LZMA_CHECK_CRC64 )
                );
        else if( eFmt == LzmaAlone )
        {
            lzma_options_lzma opt;
            lzma_lzma_preset( &opt, nCompression );
            lzmaError( lzma_alone_encoder( pState, &opt ) );
        }
        else
            throw Bu::ExceptionBase("Invalid format for lzma.");
    }
    if( bReading == true )
        throw ExceptionBase("This lzma filter is in reading mode, you can't write.");

    pState->next_in = (uint8_t *)pData;
    pState->avail_in = nBytes;
    for(;;)
    {
        pState->avail_out = nBufSize;
        pState->next_out = (uint8_t *)pBuf;

        lzmaError( lzma_code( pState, LZMA_RUN ) );

        if( pState->avail_out < nBufSize )
        {
            sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
        }
        if( pState->avail_in == 0 )
            break;
    }

    return nBytes;
}

bool Bu::Lzma::isOpen()
{
    TRACE();
    return (pState != NULL);
}

bool Bu::Lzma::isEos()
{
    TRACE();
    return bEos;
}

Bu::size Bu::Lzma::getCompressedSize()
{
    return sTotalOut;
}

