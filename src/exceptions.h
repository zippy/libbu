/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_EXCEPTIONS_H
#define BU_EXCEPTIONS_H

#include "bu/exceptionbase.h"
#include <stdarg.h>

namespace Bu
{
	subExceptionDecl( XmlException )
	subExceptionDecl( TafException )
	subExceptionDecl( FileException )
	subExceptionDecl( SocketException )
	subExceptionDecl( ConnectionException )
	subExceptionDecl( PluginException )
	subExceptionDecl( UnsupportedException )

	enum eFileException
	{
		excodeEOF
	};
		
	enum eConnectionException
	{
		excodeReadError,
		excodeWriteError,
		excodeBadReadError,
		excodeConnectionClosed,
		excodeSocketTimeout
	};
}

#endif
