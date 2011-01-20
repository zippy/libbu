/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/string.h"
#include <sys/time.h>

template<typename a>
struct tstCopy
{
	tstCopy( const a &src ) :
		src( src )
	{
	}

	void operator()()
	{
		a tmp = src;
	}

	a src;
};

template<typename f>
double runTest( f fnc )
{
	struct timeval tStart, tEnd;
	int j = 0;
	gettimeofday( &tStart, NULL );
	for(; j < 500000; j++ )
		fnc();
	gettimeofday( &tEnd, NULL );

	return (double)(tEnd.tv_sec-tStart.tv_sec)+
		(double)(tEnd.tv_usec-tStart.tv_usec)/1000000.0;
}

template<typename tst>
void fullTest( tst t )
{
	double dTotal;
	int iCount = 10;
	for( int j = 0; j < iCount; j++ )
		dTotal += runTest( t );
	printf("Average time:  %f\n", dTotal/iCount );
}

int main()
{
	Bu::String str;
	for( int j = 0; j < 500; j++ )
		str.append("Hey, this is a test string.  It will be reapeated many, many times.  How's that?");
	fullTest( tstCopy<Bu::String>( str ) );
}

