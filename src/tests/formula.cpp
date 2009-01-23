/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/formula.h"
#include <math.h>

int main( int argc, char *argv[] )
{
	if( argc < 2 )
	{
		printf("usage: %s <formula>\n", argv[0] );
		return -1;
	}
	Bu::Formula<uint32_t> uForm;
	Bu::Formula<double> dForm;

	class DblCeilFunc : public Bu::Formula<double>::Func
	{
	public:
		virtual double operator()( double x )
		{
			return ceil( x );
		}
	};

	dForm.hFunc.insert( "ceil", new DblCeilFunc() );
	
	class IntCeilFunc : public Bu::Formula<uint32_t>::Func
	{
	public:
		virtual uint32_t operator()( uint32_t x )
		{
			return x;
		}
	};

	uForm.hFunc.insert( "ceil", new IntCeilFunc() );

	uForm.hVars.insert("x", 10 );
	dForm.hVars.insert("x", 10.00 );
	uForm.hVars.insert("y", 10 );
	dForm.hVars.insert("y", 10.00 );

	for( int j = 1; j < argc; j++ )
	{
		printf("u: %s = %u\n", argv[j], uForm.run( argv[j] ) );
		printf("d: %s = %f\n", argv[j], dForm.run( argv[j] ) );
	}

	return 0;
}

