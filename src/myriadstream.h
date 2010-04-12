/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MYRIAD_STREAM_H
#define BU_MYRIAD_STREAM_H

#include "bu/stream.h"
#include "bu/myriad.h"

namespace Bu
{
	class MyriadStream : public Bu::Stream
	{
		friend class Myriad;
	private:
		/**
		 * These can only be created by the Myriad class.
		 */
		MyriadStream( Myriad &rMyriad, Myriad::Stream *pStream );

	public:
		virtual ~MyriadStream();

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

	private:
		Myriad &rMyriad;
		Myriad::Stream *pStream;
		Myriad::Block *pCurBlock;
		int iBlockSize;
		int iPos; 
	};
};

#endif
