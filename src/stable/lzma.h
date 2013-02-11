/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_LZMA_H
#define BU_LZMA_H

#include <stdint.h>

#include "bu/filter.h"

namespace Bu
{
    /**
     * Provides XZ compression and decompression, both LZMA1 (LzmaAlone) as
     * well as the newer LZMA2 (xz) format.  This uses .xz by default.
     *
     *@ingroup Streams
     *@ingroup Compression
     */
    class Lzma : public Bu::Filter
    {
    public:
        enum Format
        {
            Xz          = 0x01,
            LzmaAlone   = 0x02,
        };

        Lzma( Bu::Stream &rNext, int nCompression=6, Format eFmt=Xz );
        virtual ~Lzma();

        virtual void start();
        virtual Bu::size stop();
        virtual Bu::size read( void *pBuf, Bu::size nBytes );
        virtual Bu::size write( const void *pBuf, Bu::size nBytes );

        virtual bool isOpen();
        virtual bool isEos();

        Bu::size getCompressedSize();

    private:
        void lzmaError( int code );
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
