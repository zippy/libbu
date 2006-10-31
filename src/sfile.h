#ifndef SFILE_H
#define SFILE_H

#include <stdint.h>

#include "stream.h"

class SFile : public Stream
{
public:
	SFile( const char *sName, const char *sFlags );
	virtual ~SFile();

	virtual void close();
	virtual size_t read( char *pBuf, size_t nBytes );
	virtual size_t write( const char *pBuf, size_t nBytes );

	virtual long tell();
	virtual void seek( long offset );
	virtual void setPos( long pos );
	virtual void setPosEnd( long pos );

private:
	FILE *fh;

};

#endif
