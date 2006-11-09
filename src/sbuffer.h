#ifndef S_BUFFER_H
#define S_BUFFER_H

#include <stdint.h>

#include "stream.h"
#include "flexbuf.h"

class SBuffer : public Stream
{
public:
	SBuffer();
	virtual ~SBuffer();

	virtual void close();
	virtual size_t read( char *pBuf, size_t nBytes );

	/**
	 *@todo Update this to write at nPos, not just append data.
	 */
	virtual size_t write( const char *pBuf, size_t nBytes );

	virtual long tell();
	virtual void seek( long offset );
	virtual void setPos( long pos );
	virtual void setPosEnd( long pos );

	FlexBuf &getBuffer()
	{
		return fbData;
	}

private:
	long nPos;
	bool bOpen;
	FlexBuf fbData;
};

#endif
