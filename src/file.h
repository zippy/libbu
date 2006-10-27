#ifndef FILE_H
#define FILE_H

#include <stdint.h>

#include "stream.h"

class File : public Stream
{
public:
	File( const char *sName, const char *sFlags );
	virtual ~File();

	virtual void close();
	virtual size_t read( char *pBuf, size_t nBytes );
	virtual size_t write( char *pBuf, size_t nBytes );

private:
	FILE *fh;

};

#endif
