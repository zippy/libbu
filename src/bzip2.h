#ifndef B_ZIP2_H
#define B_ZIP2_H

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
		virtual void stop();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

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
