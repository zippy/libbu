#include "xmlexception.h"
#include <stdarg.h>

subExceptionDef( XmlException )

/*
XmlException::XmlException( const char *lpFormat, ... ) throw() :
	Exception( 0 )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

XmlException::XmlException( int nCode, const char *lpFormat, ... ) throw() :
	Exception( nCode )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

XmlException::XmlException( int nCode ) throw() :
	Exception( nCode )
{
}
*/
