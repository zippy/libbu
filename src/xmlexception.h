#ifndef XML_EXCEPTION_H
#define XML_EXCEPTION_H

#include <string>
#include "exception.h"
#include <stdarg.h>

subExceptionDecl( XmlException )

/*
class XmlException : public Exception
{
public:
	XmlException( const char *sFormat, ... ) throw();
	
	XmlException( int nCode, const char *sFormat, ... ) throw();
	
	XmlException( int nCode=0 ) throw();
};*/

#endif
