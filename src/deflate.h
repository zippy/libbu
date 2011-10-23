/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_DEFLATE_H
#define BU_DEFLATE_H

#include <stdint.h>
#include <zlib.h>

#include "bu/filter.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Streams
	 */
	class Deflate : public Bu::Filter
	{
	public:
		enum Format
		{
			Raw			= 0x01,
			Zlib		= 0x02,
			Gzip		= 0x03,
			AutoDetect	= 0x04,

			AutoRaw		= 0x04|0x01,
			AutoZlib	= 0x04|0x02,
			AutoGzip	= 0x04|0x03
		};

		Deflate( Bu::Stream &rNext, int nCompression=9, Format eFmt=AutoRaw );
		virtual ~Deflate();

		virtual void start();
		virtual Bu::size stop();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );

		virtual bool isOpen();
		virtual bool isEos();

		Bu::size getCompressedSize();

	private:
		void zError( int code );
		z_stream zState;
		bool bReading;
		int nCompression;
		char *pBuf;
		uint32_t nBufSize;
		Bu::size sTotalOut;
		Format eFmt;
		bool bEos;
	};
}

#endif
