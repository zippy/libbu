#ifndef BU_STREAM_STACK_H
#define BU_STREAM_STACK_H

#include "bu/stream.h"

namespace Bu
{
	class StreamStack : public Bu::Stream
	{
	public:
		StreamStack();
		virtual ~StreamStack();


		//
		// Everything below here merely passes on the call to the top of the
		// stream stack.
		//

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual size_t write( const Bu::FString &sBuf );
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
		typedef Bu::List<Bu::Stream *> FilterList;
		FilterList lFilts;
	};
};

#endif
