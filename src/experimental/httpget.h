/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
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

		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

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
