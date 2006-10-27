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
	virtual size_t write( char *pBuf, size_t nBytes ) = 0;

private:

};

#endif
