#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>

#include "stream.h"
#include "fstring.h"

namespace Bu
{
	/**
	 *
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
		virtual size_t write( const void *pBuf, size_t nBytes );

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

	private:
		int nSocket;
		bool bActive;
		FString sReadBuf;
	};
}

#endif
