/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STATIC_MEM_BUF_H
#define BU_STATIC_MEM_BUF_H

#include <stdint.h>

#include "bu/config.h"
#include "bu/stream.h"

namespace Bu
{
	/**
	 * An immutable, read-only memory buffer.  Construct this buffer around a
	 * block of raw memory, provide the length of the block, and you can read
	 * from that block via this class as though it were a normal stream.
	 *
	 * Use this class instead of MemBuf when you have a string already, and
	 * don't need to change it.  MemBuf will make a copy of your string for
	 * it's own use (often) and this will not (ever).
	 *@ingroup Streams
	 */
	class StaticMemBuf : public Stream
	{
	public:
		StaticMemBuf( const void *pData, size iSize );
		virtual ~StaticMemBuf();

		virtual void close();
		virtual size read( void *pBuf, size iBytes );

		virtual size write( const void *pBuf, size iBytes );
		using Stream::write;
		virtual size tell();
		virtual void seek( size offset );
		virtual void setPos( size pos );
		virtual void setPosEnd( size pos );
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
		virtual void setSize( size iSize );
		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

	private:
		const void *pData;
		size iSize;
		size nPos;
	};
}

#endif
