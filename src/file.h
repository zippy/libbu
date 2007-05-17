#ifndef FILE_H
#define FILE_H

#include <stdint.h>

#include "stream.h"

namespace Bu
{
	class File : public Bu::Stream
	{
	public:
		File( const char *sName, const char *sFlags );
		virtual ~File();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEOS();

		virtual bool canRead();
		virtual bool canWrite();
		virtual bool canSeek();

		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

	private:
		FILE *fh;

	};
}

#endif
