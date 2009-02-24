#ifndef BU_BUFFER_H
#define BU_BUFFER_H

#include "bu/filter.h"

namespace Bu
{
	class Buffer : public Bu::Filter
	{
	public:
		Buffer( Bu::Stream &rNext, int iBufSize=4096 );
		virtual ~Buffer();

		virtual void start();
		virtual size_t stop();

		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;

		virtual void flush();

	private:
		size_t sSoFar;
		int iBufSize;
		char *sReadBuf;
		char *sWriteBuf;
		int iReadBufFill;
		int iWriteBufFill;
	};
};

#endif
