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
