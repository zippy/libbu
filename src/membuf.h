#ifndef MEM_BUF_H
#define MEM_BUF_H

#include <stdint.h>

#include "bu/stream.h"

namespace Bu
{
	/**
	 * A memory buffer stream.
	 */
	class MemBuf : public Stream
	{
	public:
		MemBuf();
		virtual ~MemBuf();

		virtual void close();
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
		Bu::FString sBuf;
		long nPos;
	};
}

#endif
