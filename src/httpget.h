/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_HTTP_GET_H
#define BU_HTTP_GET_H

#include "bu/stream.h"
#include "bu/string.h"
#include "bu/url.h"
#include "bu/tcpsocket.h"
#include "bu/hash.h"

namespace Bu
{
	class HttpGet : public Bu::Stream
	{
	public:
		HttpGet( const Bu::Url &uSrc, const Bu::String &sMethod="GET" );
		virtual ~HttpGet();

		void get();

		// From Bu::Stream
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
		Bu::Url uSrc;
		Bu::String sMethod;
		Bu::TcpSocket sSrv;
		typedef Bu::Hash<Bu::String,Bu::String> MimeHash;
		MimeHash hMimeIn;
		MimeHash hMimeOut;
	};
};

#endif
