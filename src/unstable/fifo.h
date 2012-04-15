/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FIFO_H
#define BU_FIFO_H

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "bu/stream.h"
#include "bu/string.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDecl( FifoException );

	/**
	 * A fifo stream.
	 *@ingroup Streams
	 */
	class Fifo : public Bu::Stream
	{
	public:
		Fifo( const Bu::String &sName, int iFlags, mode_t mAcc=-1 );
		virtual ~Fifo();

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );
		using Stream::write;

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

		enum {
			Read		= 0x01,
			Write		= 0x02,
			Create		= 0x04,
			Delete		= 0x08,
			NonBlock	= 0x10
		};

	private:
		int iFlags;
		int iIn;
		int iOut;
	};
}

#endif
