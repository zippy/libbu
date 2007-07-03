#ifndef BU_BZIP2_H
#define BU_BZIP2_H

#include <stdint.h>
#include <bzlib.h>

#include "bu/filter.h"

namespace Bu
{
	/**
	 *
	 */
	class BZip2 : public Bu::Filter
	{
	public:
		BZip2( Bu::Stream &rNext, int nCompression=9 );
		virtual ~BZip2();

		virtual void start();
		virtual size_t stop();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual bool isOpen();

	private:
		void bzError( int code );
		bz_stream bzState;
		bool bReading;
		int nCompression;
		char *pBuf;
		uint32_t nBufSize;
	};
}

#endif
