#ifndef BU_NULL_STREAM_H
#define BU_NULL_STREAM_H

#include "bu/stream.h"

namespace Bu
{
	/**
	 * Works a lot like /dev/null on *nix style systems.  This class allows
	 * infinite reading and writing.  All operatorns "succeed" even if they
	 * don't seem to do anything.  This is great for testing writing code or
	 * doing dry runs.  When reading, it will produce NULL bytes, so any
	 * application that would like the ability to produce null streams as a
	 * snap-in replacement for any other Bu::Stream, this is the right option.
	 *
	 * As an added feature, the NullStream will track how many bytes it was
	 * asked to read and write, allowing you to use it to determine how many
	 * bytes a write opretion would use without actually writing anything.
	 */
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

		size_t getBytesRead() { return sRead; }
		size_t getByetsWritten() { return sWrote; }

	private:
		size_t sRead;
		size_t sWrote;
	};
};

#endif
