/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
		virtual Bu::size stop();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );

		virtual bool isOpen();

		virtual bool isEos();

	private:
		int iBPos;
		int iBuf;
		int iRPos;
		int iChars;
		bool bEosIn;
		Bu::size iTotalIn;
		Bu::size iTotalOut;
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
