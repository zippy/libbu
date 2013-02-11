/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/trace.h"

void Bu::__tracer( const char *pf )
{
    printf("trace: %s\n", pf );
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
