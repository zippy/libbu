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
		UdpSocket( const Bu::FString &sAddr, int iPort, int iFlags );
		virtual ~UdpSocket();

		virtual void close();
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

		virtual void setSize( long iSize );

		enum {
			// Flags
			Read		= 0x01,	///< Open udp socket for reading
			Write		= 0x02, ///< Open udp socket for writing
			ReadWrite	= 0x03, ///< Open for both read and write
			Broadcast   = 0x04, ///< Open for broadcast
		};


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
