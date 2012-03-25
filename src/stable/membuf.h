/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MEM_BUF_H
#define BU_MEM_BUF_H

#include <stdint.h>

#include "bu/config.h"
#include "bu/stream.h"
#include "bu/string.h"

namespace Bu
{
	/**
	 * A memory buffer stream.  This provides a read/write stream in memory that
	 * works exactly like a file stream...only in memory.  You can seed the
	 * memory buffer with a Bu::String of your own, or start with an empty one.
	 * Due to Bu::String using Bu::SharedCore starting with a string will not
	 * necesarilly cause the MemBuf to make a copy of your memory, but if you're
	 * sure you're not going to need to change the stream then use StaticMemBuf.
	 *@ingroup Streams
	 */
	class MemBuf : public Stream
	{
	public:
		MemBuf();
		MemBuf( const Bu::String &str );
		virtual ~MemBuf();

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

		Bu::String &getString();
		void setString( const Bu::String &sNewData );

	private:
		Bu::String sBuf;
		size nPos;
	};
}

#endif
