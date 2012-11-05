/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TCP_SOCKET_H
#define BU_TCP_SOCKET_H

#include <stdint.h>

#include "bu/config.h"
#include "bu/stream.h"
#include "bu/string.h"
#include "bu/exceptionbase.h"

namespace Bu
{
    subExceptionDeclBegin( TcpSocketException );
        enum {
            cRead,
            cWrite,
            cBadRead,
            cClosed,
            cTimeout
        };
    subExceptionDeclEnd();

    /**
     * Network socket stream class.  This class provides a mechanism for
     * communicating over a network using TCP/IP.  It will provide other low
     * level protocol and addressing support later on, but for now it's just
     * standard STREAM TCP/IP sockets.
     *
     * Unlike system sockets, these sockets are opened by default in
     * non-blocking mode, you can specify your own timeout for opening a socket,
     * and a number of non-fatal error messages have been automatically handled
     * and treated as standard no-data-available-yet situations on read.
     *
     * Please note that there is a condition that will occur eventually (at
     * least on *nix systems) that will trigger a SIGPIPE condition.  This
     * will terminate your program immediately unless handled properly.  Most
     * people doing any connections with TcpSocket will want to put this in
     * their program somewhere before they use it:
     *@code
     #include <signal.h>
     ...
     ...
     ...
     sigset( SIGPIPE, SIG_IGN ); // do this before you use a Bu::TcpSocket
     @endcode
     * When this is done, Bu::TcpSocket will simply throw a broken pipe
     * exception just like every other error condition, allowing your program
     * to handle it sanely.
     *
     *@ingroup Serving
     *@ingroup Streams
     */
    class TcpSocket : public Stream
    {
    public:
#ifdef WIN32
        typedef unsigned int handle;
#else
        typedef int handle;
#endif

        TcpSocket( handle nTcpSocket );
        TcpSocket( const String &sAddr, int nPort, int nTimeout=30,
                bool bBlocking=true );
        virtual ~TcpSocket();
        
        virtual void close();
        virtual size read( void *pBuf, size nBytes );
        virtual size read( void *pBuf, size nBytes,
                uint32_t nSec, uint32_t nUSec=0 );
        virtual size write( const void *pBuf, size nBytes );
        virtual size write( const void *pBuf, size nBytes,
                uint32_t nSec, uint32_t nUSec=0 );
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

        Bu::String getAddress() const;
        operator handle() const;

        handle getHandle() const;
        handle takeHandle();

        virtual size getSize() const;
        virtual size getBlockSize() const;
        virtual Bu::String getLocation() const;

    private:
        void setAddress();

        handle nTcpSocket;
        
        bool bActive;
        bool bBlocking;
        String sReadBuf;
        String sAddress;
    };
}

#endif
