/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "exceptionbase.h"
#include <stdarg.h>

Bu::ExceptionBase::ExceptionBase( const char *lpFormat, ... ) throw() :
	nErrorCode( 0 ),
	sWhat( NULL )
#ifdef LIBBU_EXCEPTION_BACKTRACE
	, sBT( NULL )
#endif
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
#ifdef LIBBU_EXCEPTION_BACKTRACE
	createBacktrace();
#endif
}

Bu::ExceptionBase::ExceptionBase( int nCode, const char *lpFormat, ... ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
#ifdef LIBBU_EXCEPTION_BACKTRACE
	, sBT( NULL )
#endif
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
#ifdef LIBBU_EXCEPTION_BACKTRACE
	createBacktrace();
#endif
}

Bu::ExceptionBase::ExceptionBase( int nCode ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
#ifdef LIBBU_EXCEPTION_BACKTRACE
	, sBT( NULL )
#endif
{
#ifdef LIBBU_EXCEPTION_BACKTRACE
	createBacktrace();
#endif
}

Bu::ExceptionBase::ExceptionBase( const ExceptionBase &e ) throw () :
	nErrorCode( e.nErrorCode ),
	sWhat( NULL )
#ifdef LIBBU_EXCEPTION_BACKTRACE
	, sBT( NULL )
#endif
{
	setWhat( e.sWhat );
#ifdef LIBBU_EXCEPTION_BACKTRACE
	createBacktrace();
#endif
}

Bu::ExceptionBase::~ExceptionBase() throw()
{
	delete[] sWhat;
	sWhat = NULL;
#ifdef LIBBU_EXCEPTION_BACKTRACE
	delete[] sBT;
	sBT = NULL;
#endif
}

void Bu::ExceptionBase::setWhat( const char *lpFormat, va_list &vargs )
{
	if( sWhat ) delete[] sWhat;
	int nSize;

	nSize = vsnprintf( NULL, 0, lpFormat, vargs );
	sWhat = new char[nSize+1];
	vsnprintf( sWhat, nSize+1, lpFormat, vargs );
}

void Bu::ExceptionBase::setWhat( const char *lpText )
{
	if( sWhat ) delete[] sWhat;
	int nSize;

	nSize = strlen( lpText );
	sWhat = new char[nSize+1];
	strcpy( sWhat, lpText );
}

const char *Bu::ExceptionBase::what() const throw()
{
	return sWhat;
}

int Bu::ExceptionBase::getErrorCode()
{
	return nErrorCode;
}

#ifdef LIBBU_EXCEPTION_BACKTRACE
const char *Bu::ExceptionBase::getBacktrace() const throw()
{
	return sBT;
}

#include "bu/fstring.h"
#include <execinfo.h>

void Bu::ExceptionBase::createBacktrace()
{
	void *array[1000];
	size_t size;
	char **strings;
	size_t i;
	
	size = backtrace (array, 1000);
	strings = backtrace_symbols (array, size);

	Bu::FString s;
	s.format("Obtained %zd stack frames.\n", size );

	for (i = 0; i < size; i++)
	{
		s += strings[i];
		s += "\n";
	}

	free (strings);

	sBT = new char[s.getSize()+1];
	strcpy( sBT, s.getStr() );
}

#else
const char *Bu::ExceptionBase::getBacktrace() const throw()
{
	return "Backtrace support is not compiled in.\n";
}
#endif
