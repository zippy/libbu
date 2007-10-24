#ifndef BU_SOCKET_H
#define BU_SOCKET_H

#include <stdint.h>

#include "stream.h"
#include "fstring.h"

namespace Bu
{
	/**
	 *@author Mike Buland
	 *@ingroup Serving Streams
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

	private:
		void setAddress();

		int nSocket;
		bool bActive;
		FString sReadBuf;
		FString sAddress;
	};
}

#endif
