/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/trace.h"

void Bu::__tracer( const char *pf )
{
	printf("trace: %s\n", pf );
}

template<> void Bu::__tracer_format<int8_t>( const int8_t &v )
{
	printf("%hhd", v );
}

template<> void Bu::__tracer_format<uint8_t>( const uint8_t &v )
{
	printf("%hhu", v );
}

template<> void Bu::__tracer_format<int16_t>( const int16_t &v )
{
	printf("%hd", v );
}

template<> void Bu::__tracer_format<uint16_t>( const uint16_t &v )
{
	printf("%hu", v );
}

template<> void Bu::__tracer_format<int32_t>( const int32_t &v )
{
	printf("%d", v );
}

template<> void Bu::__tracer_format<uint32_t>( const uint32_t &v )
{
	printf("%u", v );
}

template<> void Bu::__tracer_format<int64_t>( const int64_t &v )
{
	printf("%lld", v );
}

template<> void Bu::__tracer_format<uint64_t>( const uint64_t &v )
{
	printf("%llu", v );
}

template<> void Bu::__tracer_format<bool>( const bool &v )
{
	if( v )
		printf("true");
	else
		printf("false");
}

template<> void Bu::__tracer_format<char>( const char &v )
{
	printf("%hhd", v );
}

template<> void Bu::__tracer_format<long>( const long &v )
{
	printf("%ld", v );
}

template<> void Bu::__tracer_format<unsigned long>( const unsigned long &v )
{
	printf("%lu", v );
}

template<> void Bu::__tracer_format<float>( const float &v )
{
	printf("%f", v );
}

template<> void Bu::__tracer_format<double>( const double &v )
{
	printf("%f", v );
}

template<> void Bu::__tracer_format<void *>( void * const &v )
{
	printf("0x%08X", (ptrdiff_t)v );
}

template<> void Bu::__tracer_format<char *>( char * const &v )
{
	printf("\"%s\"", v );
}

template<> void Bu::__tracer_format<char **>( char ** const &v )
{
	printf("[");
	for( int j = 0; v[j]; j++ )
	{
		if( j > 0 )
			printf(", ");
		printf("\"%s\"", v[j] );
	}
	printf("]");
}

template<> void Bu::__tracer_format<void const *>( void const * const &v )
{
	printf("0x%08X", (ptrdiff_t)v );
}

template<> void Bu::__tracer_format<char const *>( char const * const &v )
{
	printf("\"%s\"", v );
}

template<> void Bu::__tracer_format<char const **>( char const ** const &v )
{
	printf("[");
	for( int j = 0; v[j]; j++ )
	{
		if( j > 0 )
			printf(", ");
		printf("\"%s\"", v[j] );
	}
	printf("]");
}
