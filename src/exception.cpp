#include "exception.h"
#include <stdarg.h>

Exception::Exception( const char *lpFormat, ... ) throw() :
	nErrorCode( 0 )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

Exception::Exception( int nCode, const char *lpFormat, ... ) throw() :
	nErrorCode( nCode )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

Exception::Exception( int nCode ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
{
}

Exception::~Exception() throw()
{
	delete[] sWhat;
}

void Exception::setWhat( const char *lpFormat, va_list &vargs )
{
	int nSize;

	nSize = vsnprintf( NULL, 0, lpFormat, vargs );
	sWhat = new char[nSize+1];
	vsnprintf( sWhat, nSize+1, lpFormat, vargs );
}

const char *Exception::what() const throw()
{
	return sWhat;
}

int Exception::getErrorCode()
{
	return nErrorCode;
}

