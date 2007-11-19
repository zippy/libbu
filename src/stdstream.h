/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STD_STREAM_H
#define BU_STD_STREAM_H

#include <stdint.h>
#include <stdio.h>
#include "stream.h"

namespace Bu
{
	class StdStream : public Stream
	{
	public:
		StdStream();
		virtual ~StdStream();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEOS();
		virtual bool isOpen();
		virtual void flush();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();
		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );
	};
}

#endif
