/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FILTER_H
#define BU_FILTER_H

#include <stdint.h>

#include "bu/stream.h"

namespace Bu
{
	/**
	 * Data filter base class.  Each data filter should contain a read and write
	 * section.  Effectively, the write applies the filter, the read un-applies
	 * the filter, if possible.  For example, BZip2 is a filter that compresses
	 * on write and decompresses on read.  All bi-directional filters should
	 * follow:  x == read( write( x ) ) (byte-for-byte comparison)
	 *
	 * Also, all returned buffers should be owned by the filter, and deleted
	 * when the filter is deleted.  This means that the output of a read or
	 * write operation must be used before the next call to read or write or the
	 * data will be destroyed.  Also, the internal buffer may be changed or
	 * recreated between calls, so always get a new pointer from a call to
	 * read or write.
	 *
	 * The close function can also return data, so make sure to check for it,
	 * many filters such as compression filters will buffer data until they have
	 * enough to create a compression block, in these cases the leftover data
	 * will be returned by close.
	 *@ingroup Streams
	 */
	class Filter : public Bu::Stream
	{
	public:
		Filter( Bu::Stream &rNext );
		virtual ~Filter();

		virtual void start()=0;
		virtual Bu::size stop()=0;
		virtual void close();
		virtual Bu::size tell();
		virtual void seek( Bu::size offset );
		virtual void setPos( Bu::size pos );
		virtual void setPosEnd( Bu::size pos );
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

		/**
		 * Most filters won't re-implement this, it doesn't make a lot of sense
		 * for filters, in general.
		 */
		virtual void setSize( Bu::size iSize );

		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;
		
	protected:
		Bu::Stream &rNext;

	private:

	};
}

#endif
