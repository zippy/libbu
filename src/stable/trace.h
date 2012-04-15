/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TRACE_H
#define BU_TRACE_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <bu/sio.h>

namespace Bu
{
/*	template<typename t> void __tracer_format( t &v )
	{
		__tracer_format( *const_cast<const t *>(&v) );
	}
*/
	template<typename t> void __tracer_format( const t &v )
	{
		Bu::sio << v;
	}

	void __tracer( const char *pf );

	#define looper( vv ) 								\
		for( ; *n; n++ ) 								\
		{												\
			if( bInBracket == true )					\
			{											\
				if( *n == '>' )							\
					bInBracket = false;					\
			}											\
			else if( *n == ',' || *n == ')' )			\
			{											\
				fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, 1, stdout );	\
				fwrite("=", 1, 1, stdout);				\
				__tracer_format( vv );					\
				s = n;									\
				n++;									\
				break;									\
			}											\
			else if( *n == '<' )						\
			{											\
				bInBracket = true;						\
			}											\
		} (void)0
	
	template<typename t1> void __tracer( const char *pf, t1 &v1 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2> void __tracer( const char *pf,
		t1 &v1, t2 &v2 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2, typename t3>
	void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2, typename t3, typename t4>
	void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2, typename t3, typename t4, typename t5>
	void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2, typename t3, typename t4, typename t5,
	typename t6>
	void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5,
		t6 &v6)
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		looper( v6 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
	
	template<typename t1, typename t2, typename t3, typename t4, typename t5,
	typename t6, typename t7>
	void __tracer( const char *pf, t1 &v1, t2 &v2, t3 &v3, t4 &v4, t5 &v5,
		t6 &v6, t7 &v7 )
	{
		printf("trace: ");
		const char *s = pf;
		const char *n = pf;
		bool bInBracket = false;
		looper( v1 );
		looper( v2 );
		looper( v3 );
		looper( v4 );
		looper( v5 );
		looper( v6 );
		looper( v7 );
		fwrite( s, (ptrdiff_t)n-(ptrdiff_t)s, strlen(s), stdout );
		fwrite( "\n", 1, 1, stdout );
		fflush( stdout );
	}
#undef looper
	
	template<> void __tracer_format<float>( const float &v );
	template<> void __tracer_format<double>( const double &v );
	template<> void __tracer_format<void *>( void * const &v );
	template<> void __tracer_format<char *>( char * const &v );
	template<> void __tracer_format<char **>( char ** const &v );
	template<> void __tracer_format<void const *>( void const * const &v );
	template<> void __tracer_format<char const *>( char const * const &v );
	template<> void __tracer_format<char const **>( char const ** const &v );
}

#ifdef BU_TRACE
# define TRACE(args...) Bu::__tracer( __PRETTY_FUNCTION__, ##args )
#else
# define TRACE(args...) (void)0
#endif

#endif
