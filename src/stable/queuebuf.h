/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_QUEUE_BUF_H
#define BU_QUEUE_BUF_H

#include "bu/stream.h"

namespace Bu
{
    /**
     * A queuing buffer stream class.  All data written to this class is
     * appended to it, there is no stored position.  All data read is read
     * from the begining and then thrown away.  It operates by using a linked
     * list of small buffers, and deallocating or reusing them when it can.
     */
    class QueueBuf : public Bu::Stream
    {
    public:
        QueueBuf( int iBlockSize=256 );
        virtual ~QueueBuf();

        virtual void close();
        virtual Bu::size read( void *pBuf, Bu::size nBytes );
        virtual Bu::size peek( void *pBuf, Bu::size nBytes );
        virtual Bu::size peek( void *pBuf, Bu::size nBytes, Bu::size nSkip );
        virtual Bu::size write( const void *pBuf, Bu::size nBytes );
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
        void addBlock();
        void removeBlock();

    private:
        int iBlockSize;
        int iReadOffset;
        int iWriteOffset;
        Bu::size iTotalSize;
        typedef Bu::List<char *> BlockList;
        BlockList lBlocks;
    };
};

#endif
