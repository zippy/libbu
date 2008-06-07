/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "exceptionbase.h"
#include <stdarg.h>
#include <string.h>

Bu::ExceptionBase::ExceptionBase( const char *lpFormat, ... ) throw() :
	nErrorCode( 0 ),
	sWhat( NULL )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

Bu::ExceptionBase::ExceptionBase( int nCode, const char *lpFormat, ... ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
{
	va_list ap;

	va_start(ap, lpFormat);
	setWhat( lpFormat, ap );
	va_end(ap);
}

Bu::ExceptionBase::ExceptionBase( int nCode ) throw() :
	nErrorCode( nCode ),
	sWhat( NULL )
{
}

Bu::ExceptionBase::ExceptionBase( const ExceptionBase &e ) throw () :
	nErrorCode( e.nErrorCode ),
	sWhat( NULL )
{
	setWhat( e.sWhat );
}

Bu::ExceptionBase::~ExceptionBase() throw()
{
	delete[] sWhat;
	sWhat = NULL;
}

void Bu::ExceptionBase::setWhat( const char *lpFormat, va_list &vargs )
{
	if( sWhat ) delete[] sWhat;
	int nSize;

	va_list vargs2;
	va_copy( vargs2, vargs );
	nSize = vsnprintf( NULL, 0, lpFormat, vargs2 );
	va_end( vargs2 );
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

