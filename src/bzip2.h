/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_BZIP2_H
#define BU_BZIP2_H

#include <stdint.h>
#include <bzlib.h>

#include "bu/filter.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Streams
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

		size_t getCompressedSize();

	private:
		void bzError( int code );
		bz_stream bzState;
		bool bReading;
		int nCompression;
		char *pBuf;
		uint32_t nBufSize;
		size_t sTotalOut;
	};
}

#endif
