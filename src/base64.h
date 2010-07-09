/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_BASE64_H
#define BU_BASE64_H

#include "bu/filter.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDecl( Base64Exception );
	
	/**
	 *
	 *@ingroup Streams
	 */
	class Base64 : public Bu::Filter
	{
	public:
		Base64( Bu::Stream &rNext );
		virtual ~Base64();

		virtual void start();
		virtual size_t stop();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );

		virtual bool isOpen();

		virtual bool isEos();

	private:
		int iBPos;
		int iBuf;
		int iRPos;
		int iChars;
		bool bEosIn;
		size_t iTotalIn;
		size_t iTotalOut;
		static const char tblEnc[65];
		char tblDec[80];
		enum Mode
		{
			Nothing		= 0x00,
			Encode		= 0x01,
			Decode		= 0x02,
		};
		Mode eMode;
	};
};

#endif
