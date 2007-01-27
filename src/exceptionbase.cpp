#include "exceptionbase.h"
#include <stdarg.h>

ExceptionBase::ExceptionBase( const char *lpFormat, ... ) throw() :
	nErrorCode( 0 ),
	sWhat( NULL )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

ExceptionBase::ExceptionBase( int nCode, const char *lpFormat, ... ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

ExceptionBase::ExceptionBase( int nCode ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
{
}

ExceptionBase::~ExceptionBase() throw()
{
	if( sWhat )
	{
		delete[] sWhat;
		sWhat = NULL;
	}
}

void ExceptionBase::setWhat( const char *lpFormat, va_list &vargs )
{
	if( sWhat ) delete[] sWhat;
	int nSize;

	nSize = vsnprintf( NULL, 0, lpFormat, vargs );
	sWhat = new char[nSize+1];
	vsnprintf( sWhat, nSize+1, lpFormat, vargs );
}

void ExceptionBase::setWhat( const char *lpText )
{
	if( sWhat ) delete[] sWhat;
	int nSize;

	nSize = strlen( lpText );
	sWhat = new char[nSize+1];
	strcpy( sWhat, lpText );
}

const char *ExceptionBase::what() const throw()
{
	return sWhat;
}

int ExceptionBase::getErrorCode()
{
	return nErrorCode;
}

