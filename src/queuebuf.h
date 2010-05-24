/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_QUEUE_BUF_H
#define BU_QUEUE_BUF_H

#include "bu/stream.h"

namespace Bu
{
	/**
	 * A queuing buffer stream class.  All data written to this class is
	 * appended to it, there is no stored position.  All data read is read
	 * from the begining and then thrown away.  It operates by using a linked
	 * list of small buffers, and deallocating or reusing them when it can.
	 */
	class QueueBuf : public Bu::Stream
	{
	public:
		QueueBuf( int iBlockSize=256 );
		virtual ~QueueBuf();

		int getSize();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t peek( void *pBuf, size_t nBytes );
		virtual size_t peek( void *pBuf, size_t nBytes, size_t nSkip );
		virtual size_t write( const void *pBuf, size_t nBytes );
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
		virtual void setSize( long iSize );

	private:
		void addBlock();
		void removeBlock();

	private:
		int iBlockSize;
		int iReadOffset;
		int iWriteOffset;
		size_t iTotalSize;
		typedef Bu::List<char *> BlockList;
		BlockList lBlocks;
	};
};

#endif
