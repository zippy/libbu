#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "exceptionbase.h"
#include <stdarg.h>

namespace Bu
{
	subExceptionDecl( XmlException )
	subExceptionDecl( FileException )
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
