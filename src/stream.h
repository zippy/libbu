#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stdio.h>

namespace Bu
{
	/**
	 * The basis for a completely general data transport mechanism.  Anything
	 * that inherits from this should provide at least the basic read and/or
	 * write functions, and very probably the close function.  Any functions
	 * that aren't supported should throw an exception if called.
	 *
	 * The constructor of a child class should pretty much universally be used
	 * to open the stream.  I can't think of anything that should require an
	 * exception.
	 */
	class Stream
	{
	public:
		Stream();
		virtual ~Stream();

		virtual void close() = 0;
		virtual size_t read( void *pBuf, size_t nBytes ) = 0;
		virtual size_t write( const void *pBuf, size_t nBytes ) = 0;

		virtual long tell() = 0;
		virtual void seek( long offset ) = 0;
		virtual void setPos( long pos ) = 0;
		virtual void setPosEnd( long pos ) = 0;
		virtual bool isEOS() = 0;
		virtual bool isOpen() = 0;

		virtual void flush() = 0;

		/**
		 * In non-blocking streams this indicates if a read operation will
		 * return data at the moment or not.  In blocking streams this should
		 * return the same value as isEOS().
		 */
		virtual bool canRead() = 0;

		/**
		 * In non-blocking streams this indicates if a write operation will
		 * succeed or fail.  In some cases writing is not allowed (e.g. 
		 * internal buffers are full) temporarilly.  In blocking streams this
		 * should return the same value as isWritable.
		 */
		virtual bool canWrite() = 0;

		/**
		 * Indicates if the stream is capable of read operations.  This does not
		 * indicate if such operations will return useful data, see canRead for
		 * that.
		 */
		virtual bool isReadable() = 0;

		/**
		 * Indicates if the stream is capable of write operations.  This does
		 * not indicate if such operations will succeed or fail, see canWrite
		 * for that.
		 */
		virtual bool isWritable() = 0;

		/**
		 * Indicates if the stream is capable of seek operations.  This is
		 * generally false for non-blocking streams.  Some buffered streams may
		 * support limited in-buffer seeking.
		 */
		virtual bool isSeekable() = 0;

		virtual bool isBlocking() = 0;
		virtual void setBlocking( bool bBlocking=true ) = 0;

	public: // Filters


	private:

	};
}

#endif
