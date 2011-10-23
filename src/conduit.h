/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CONDUIT_H
#define BU_CONDUIT_H

#include "bu/stream.h"
#include "bu/string.h"
#include "bu/queuebuf.h"
#include "bu/mutex.h"
#include "bu/condition.h"

namespace Bu
{
	/**
	 * Simple inter-thread communication stream.  This acts like a pair of
	 * pipes for stream communication between any two things, but without the
	 * use of pipes, making this a bad choice for IPC.
	 */
	class Conduit : public Stream
	{
	public:
		Conduit( int iBlockSize=256 );
		virtual ~Conduit();

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size peek( void *pBuf, Bu::size nBytes );
		virtual Bu::size peek( void *pBuf, Bu::size nBytes, Bu::size nSkip );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );
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

	private:
		QueueBuf qb;
		mutable Mutex im;
		Condition cBlock;
		bool bBlocking;
		bool bOpen;
	};
}

#endif
