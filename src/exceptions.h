#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "exceptionbase.h"
#include <stdarg.h>

subExceptionDecl( XmlException )
subExceptionDecl( FileException )
subExceptionDecl( ConnectionException )
subExceptionDecl( PluginException )

enum eFileException
{
	excodeEOF
};
	
enum eConnectionException
{
	excodeReadError,
	excodeBadReadError,
	excodeConnectionClosed,
	excodeSocketTimeout
};

#endif
