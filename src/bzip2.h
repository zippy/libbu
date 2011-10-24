/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_BZIP2_H
#define BU_BZIP2_H

#include <stdint.h>

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
		virtual Bu::size stop();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );

		virtual bool isOpen();

		Bu::size getCompressedSize();

	private:
		void bzError( int code );
		void *prState;
		bool bReading;
		int nCompression;
		char *pBuf;
		uint32_t nBufSize;
		Bu::size sTotalOut;
	};
}

#endif
