/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SOCKET_H
#define BU_SOCKET_H

#include <stdint.h>

#include "bu/stream.h"
#include "bu/fstring.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDeclBegin( SocketException );
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
	 * people doing any connections with Socket will want to put this in their
	 * program somewhere before they use it:
	 *@code
	 #include <signal.h>
	 ...
	 ...
	 ...
	 sigset( SIGPIPE, SIG_IGN ); // do this before you use a Bu::Socket
	 @endcode
	 * When this is done, Bu::Socket will simply throw a broken pipe exception
	 * just like every other error condition, allowing your program to handle
	 * it sanely.
	 *
	 *@ingroup Serving
	 *@ingroup Streams
	 */
	class Socket : public Stream
	{
	public:
		Socket( int nSocket );
		Socket( const FString &sAddr, int nPort, int nTimeout=30 );
		virtual ~Socket();
		
		virtual void close();
		//virtual void read();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t read( void *pBuf, size_t nBytes,
				uint32_t nSec, uint32_t nUSec=0 );
		virtual size_t write( const void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes,
				uint32_t nSec, uint32_t nUSec=0 );
		using Stream::write;

		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEOS();
		virtual bool isOpen();

		virtual void flush();

		virtual bool canRead();
		virtual bool canWrite();

		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();

		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

		Bu::FString getAddress() const;
		operator int() const;

	private:
		void setAddress();

#ifdef WIN32
		unsigned int nSocket;
#else
		int nSocket;
#endif
		bool bActive;
		FString sReadBuf;
		FString sAddress;
	};
}

#endif
