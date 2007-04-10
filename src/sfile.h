#ifndef SFILE_H
#define SFILE_H

#include <stdint.h>

#include "stream.h"

namespace Bu
{
	class SFile : public Bu::Stream
	{
	public:
		SFile( const char *sName, const char *sFlags );
		virtual ~SFile();

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

	private:
		FILE *fh;

	};
}

#endif
