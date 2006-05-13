#include "exception.h"
#include <stdarg.h>

Exception::Exception( const char *lpFormat, ... ) throw() :
	nErrorCode( 0 )
{
	va_list ap;
	int nSize;

	va_start(ap, lpFormat);
	nSize = vsnprintf( NULL, 0, lpFormat, ap );
	sWhat = new char[nSize+1];
	vsnprintf( sWhat, nSize+1, lpFormat, ap );
	va_end(ap);
}

Exception::Exception( int nCode, const char *lpFormat, ... ) throw() :
	nErrorCode( nCode )
{
	va_list ap;
	int nSize;

	va_start(ap, lpFormat);
	nSize = vsnprintf( NULL, 0, lpFormat, ap );
	sWhat = new char[nSize+1];
	vsnprintf( sWhat, nSize+1, lpFormat, ap );
	va_end(ap);
}

Exception::~Exception() throw()
{
	delete[] sWhat;
}

const char *Exception::what() const throw()
{
	return sWhat;
}

int Exception::getErrorCode()
{
	return nErrorCode;
}

