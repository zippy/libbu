/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_BUFFER_H
#define BU_BUFFER_H

#include "bu/filter.h"

namespace Bu
{
	class Buffer : public Bu::Filter
	{
	public:
		Buffer( Bu::Stream &rNext, int iBufSize=4096 );
		virtual ~Buffer();

		virtual void start();
		virtual size_t stop();

		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;

		size_t getReadFill() { return iReadBufFill; }
		bool isWritePending() { return iWriteBufFill > 0; }

		virtual void flush();

		virtual bool isEos();

	private:
		void fillReadBuf();

	private:
		size_t sSoFar;
		int iBufSize;
		char *sReadBuf;
		char *sWriteBuf;
		int iReadBufFill;
		int iReadPos;
		int iWriteBufFill;
		int iWritePos;
	};
};

#endif
