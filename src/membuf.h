/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MEM_BUF_H
#define BU_MEM_BUF_H

#include <stdint.h>

#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 * A memory buffer stream.
	 *@ingroup Streams
	 */
	class MemBuf : public Stream
	{
	public:
		MemBuf();
		MemBuf( const Bu::FString &str );
		virtual ~MemBuf();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );

		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;
		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEos();
		virtual bool isOpen();
		virtual void flush();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();
		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

		Bu::FString &getString();

	private:
		Bu::FString sBuf;
		long nPos;
	};
}

#endif
