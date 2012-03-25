/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef WIN32 //not on windows

#ifndef BU_UDP_SOCKET_H
#define BU_UDP_SOCKET_H

#include <stdint.h>

#include "bu/stream.h"

namespace Bu
{
	subExceptionDecl( UdpSocketException );

	class UdpSocket : public Stream
	{
	public:
		UdpSocket( int iUdpSocket );
		UdpSocket( const Bu::String &sAddr, int iPort, int iFlags );
		virtual ~UdpSocket();

		typedef uint32_t addr;

		static Bu::String addrToStr( const addr &a );

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size read( void *pBuf, Bu::size nBytes,
				addr &sHost, int &iPort );
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

		enum {
			// Flags
			Read		= 0x01,	///< Open udp socket for reading
			Write		= 0x02, ///< Open udp socket for writing
			ReadWrite	= 0x03, ///< Open for both read and write
			Broadcast   = 0x04, ///< Open for broadcast
		};

		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

	private:
#ifdef WIN32
		unsigned int iUdpSocket;
#else
		int iUdpSocket;
#endif
		void *paTarget;
		bool bBound;
	};
};

#endif

#endif
