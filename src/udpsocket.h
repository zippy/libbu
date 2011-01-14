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
		UdpSocket( const Bu::FString &sAddr, int iPort, bool bBroadcast );
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

	private:
#ifdef WIN32
		unsigned int iUdpSocket;
#else
		int iUdpSocket;
#endif
	};
};

#endif
