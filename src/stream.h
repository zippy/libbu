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

		virtual bool canRead() = 0;
		virtual bool canWrite() = 0;
		virtual bool canSeek() = 0;

	private:

	};
}

#endif
