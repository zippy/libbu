/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_DEFLATE_H
#define BU_DEFLATE_H

#include <stdint.h>

#include "bu/filter.h"

namespace Bu
{
    /**
     * Provides Deflate (LZ77) support via zlib.  This provides zlib, raw, and
     * gzip stream types.  By default it will autodetect the input type and
     * encode into a raw deflate stream.
     *
     *@ingroup Streams
     *@ingroup Compression
     */
    class Deflate : public Bu::Filter
    {
    public:
        enum Format
        {
            Raw         = 0x01,
            Zlib        = 0x02,
            Gzip        = 0x03,
            AutoDetect  = 0x04,

            AutoRaw     = 0x04|0x01,
            AutoZlib    = 0x04|0x02,
            AutoGzip    = 0x04|0x03
        };

        Deflate( Bu::Stream &rNext, int nCompression=-1, Format eFmt=AutoZlib );
        virtual ~Deflate();

        virtual void start();
        virtual Bu::size stop();
        virtual Bu::size read( void *pBuf, Bu::size nBytes );
        virtual Bu::size write( const void *pBuf, Bu::size nBytes );

        virtual bool isOpen();
        virtual bool isEos();

        Bu::size getCompressedSize();

    private:
        void zError( int code );
        void *prState;
        bool bReading;
        int nCompression;
        char *pBuf;
        uint32_t nBufSize;
        Bu::size sTotalOut;
        Format eFmt;
        bool bEos;
    };
}

#endif
