/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_NULL_STREAM_H
#define BU_NULL_STREAM_H

#include "bu/stream.h"

namespace Bu
{
	/**
	 * Works a lot like /dev/null on *nix style systems.  This class allows
	 * infinite reading and writing.  All operatorns "succeed" even if they
	 * don't seem to do anything.  This is great for testing writing code or
	 * doing dry runs.  When reading, it will produce NULL bytes, so any
	 * application that would like the ability to produce null streams as a
	 * snap-in replacement for any other Bu::Stream, this is the right option.
	 *
	 * As an added feature, the NullStream will track how many bytes it was
	 * asked to read and write, allowing you to use it to determine how many
	 * bytes a write opretion would use without actually writing anything.
	 */
	class NullStream : public Bu::Stream
	{
	public:
		NullStream();
		virtual ~NullStream();

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::String readLine();
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );
		using Bu::Stream::write;
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
		virtual void setSize( Bu::size iSize );

		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

		Bu::size getBytesRead() { return sRead; }
		Bu::size getByetsWritten() { return sWrote; }

	private:
		Bu::size sRead;
		Bu::size sWrote;
	};
};

#endif
