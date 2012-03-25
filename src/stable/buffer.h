/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
		Buffer( Bu::Stream &rNext, int iWhat=Both, int iBufSize=4096 );
		virtual ~Buffer();

		enum
		{
			Write	= 1,
			Read	= 2,
			Both	= 3
		};

		virtual void start();
		virtual Bu::size stop();

		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );
		using Stream::write;

		Bu::size getReadFill() { return iReadBufFill; }
		bool isWritePending() { return iWriteBufFill > 0; }

		virtual void flush();

		virtual bool isEos();

	private:
		void fillReadBuf();

	private:
		Bu::size sSoFar;
		int iBufSize;
		char *sReadBuf;
		char *sWriteBuf;
		int iReadBufFill;
		int iReadPos;
		int iWriteBufFill;
		int iWritePos;
		int iWhat;
	};
};

#endif
