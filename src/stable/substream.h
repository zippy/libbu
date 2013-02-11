/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SUB_STREAM_H
#define BU_SUB_STREAM_H

#include "bu/filter.h"

namespace Bu
{
    /**
     * Creates a sub-stream of a given stream.  This allows you to read and
     * write safely to a section of another stream, keeping all data within
     * the given bounds.  The substream acts exactly like a top level stream
     * when you reach the bounds of either the containing stream or the
     * artificial bounds of the substream, except that unlike many stream types,
     * when writing you cannot move beyond the bounds of the substream.  Reads,
     * on the other hand, work exactly the same way, returning less data than
     * requested when the end of the stream is reached.
     *
     * The substream always begins at the current position in the base stream,
     * if you would like to skip some data first, simply seek.
     *
     * The substream class is safe to use with all blocking and non-blocking
     * base streams, including sockets, however it can have unpredictable
     * results when used on a buffering stream that may read more data than
     * requested in order to complete a request such as the buffer or bzip2
     * filters.
     */
    class SubStream : public Bu::Filter
    {
    public:
        SubStream( Bu::Stream &rNext, Bu::size iSize );
        virtual ~SubStream();

        virtual Bu::size read( void *pBuf, Bu::size nBytes );
        virtual Bu::size write( const void *pBuf, Bu::size nBytes );
        using Bu::Stream::write;

        virtual void start();
        virtual Bu::size stop();
        virtual void close();
        virtual Bu::size tell();
        virtual void seek( Bu::size offset );
        virtual void setPos( Bu::size pos );
        virtual void setPosEnd( Bu::size pos );
        virtual bool isEos();

        virtual bool canRead();
        virtual bool canWrite();

    protected:
        Bu::size iStart;
        Bu::size iPos;
        Bu::size iSize;
    };
};

#endif
