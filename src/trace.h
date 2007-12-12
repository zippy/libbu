/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TRACE_H
#define BU_TRACE_H

#ifdef BU_TRACE
#include "bu/fstring.h"

namespace Nango
{
	template<typename t> void __tracer_format( t &v );
	
	void __tracer( const char *pf )
	{
		printf("trace: %s\n", pf );
	}

#define looper( vv ) 									\
		for( ; *n; n++ ) 								\
		{												\
			if( *n == ',' || *n == ')' )				\
			{											\
				fwrite( s, (int)n-(int)s, 1, stdout );	\
				fwrite("=", 1, 1, stdout);				\
				__tracer_format( vv );					\
				s = n;									\
				n++;									\
				break;									\
			}											\
		}
	template<typename t1> void __tracer( const char *pf, t1 &v1 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2> void __tracer( const char *pf,
			t1 &v1, t2 &v2 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2, typename t3>
		void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2, typename t3, typename t4>
		void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2, typename t3, typename t4, typename t5>
		void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2, typename t3, typename t4, typename t5,
		typename t6>
		void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5,
				t6 &v6)
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		looper( v6 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<typename t1, typename t2, typename t3, typename t4, typename t5,
		typename t6, typename t7>
		void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5,
				t6 &v6, t7 &v7 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		looper( v6 );
		looper( v7 );
		fwrite( s, (int)n-(int)s, 1, stdout );
		fwrite( "\n", 1, 1, stdout );
	}

	template<> void __tracer_format<const int>( const int &v )
	{
		printf("%d", v );
	}

	template<> void __tracer_format<int>( int &v )
	{
		printf("%d", v );
	}

	template<> void __tracer_format<void *>( void * &v )
	{
		printf("0x%08X", (unsigned int)v );
	}

	template<> void __tracer_format<const char *>( const char * &v )
	{
		printf("\"%s\"", v );
	}
}

# define TRACE(args...) Nango::__tracer( __PRETTY_FUNCTION__, ##args )
#else
# define TRACE(args...) {}
#endif

#endif
