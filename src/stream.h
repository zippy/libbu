#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>
#include <stdio.h>

class Stream
{
public:
	Stream();
	virtual ~Stream();

	virtual void close() = 0;
	virtual size_t read( char *pBuf, size_t nBytes ) = 0;
	virtual size_t write( const char *pBuf, size_t nBytes ) = 0;

	virtual long tell() = 0;
	virtual void seek( long offset ) = 0;
	virtual void setPos( long pos ) = 0;
	virtual void setPosEnd( long pos ) = 0;

private:

};

#endif
