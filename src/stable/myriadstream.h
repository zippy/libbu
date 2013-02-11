/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MYRIAD_STREAM_H
#define BU_MYRIAD_STREAM_H

#include "bu/stream.h"
#include "bu/myriad.h"

namespace Bu
{
    class MyriadStream : public Bu::Stream
    {
        friend class Myriad;
    private:
        /**
         * These can only be created by the Myriad class.
         */
        MyriadStream( Myriad &rMyriad, Myriad::Stream *pStream );

    public:
        virtual ~MyriadStream();

        virtual void close();
        virtual Bu::size read( void *pBuf, Bu::size nBytes );
        virtual Bu::size write( const void *pBuf, Bu::size nBytes );
        using Stream::write;
        virtual Bu::size tell();
        virtual void seek( Bu::size offset );
        virtual void setPos( Bu::size pos );
        virtual void setPosEnd( Bu::size pos );
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
        virtual void setSize( Bu::size iSize );

        virtual size getSize() const;
        virtual size getBlockSize() const;
        virtual Bu::String getLocation() const;

    private:
        Myriad &rMyriad;
        Myriad::Stream *pStream;
        Myriad::Block *pCurBlock;
        int iBlockSize;
        int iPos; 
    };
};

#endif
