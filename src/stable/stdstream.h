/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STD_STREAM_H
#define BU_STD_STREAM_H

#include <stdint.h>
#include "stream.h"

namespace Bu
{
    /**
     *@ingroup Streams
     */
    class StdStream : public Stream
    {
    public:
        enum OutMode
        {
            StdOut,
            StdError
        };
        StdStream( OutMode eOut=StdOut );
        virtual ~StdStream();

        virtual void close();
        virtual size read( void *pBuf, size nBytes );
        virtual size write( const void *pBuf, size nBytes );
        using Stream::write;
        virtual size tell();
        virtual void seek( size offset );
        virtual void setPos( size pos );
        virtual void setPosEnd( size pos );
        virtual bool isEos();
        virtual bool isOpen();
        virtual void flush();
        virtual bool canRead();
        virtual bool canWrite();
        virtual bool isReadable();
        virtual bool isWritable();
        virtual bool isSeekable();
        virtual bool isBlocking();
        virtual void setBlocking( bool bBlocking=true );
        virtual void setSize( size iSize );
        virtual size getSize() const;
        virtual size getBlockSize() const;
        virtual Bu::String getLocation() const;
    
    private:
        OutMode eOut;
    };
}

#endif
