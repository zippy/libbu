#ifndef BU_NULL_STREAM_H
#define BU_NULL_STREAM_H

#include "bu/stream.h"

namespace Bu
{
	class NullStream : public Bu::Stream
	{
	public:
		NullStream();
		virtual ~NullStream();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual Bu::FString readLine();
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Bu::Stream::write;
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
	};
};

#endif
