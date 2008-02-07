/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "hash.h"

namespace Bu { subExceptionDef( HashException ) }

template<>
uint32_t Bu::__calcHashCode<const char *>( const char * const &k )
{
	if (k == NULL)
	{
		return 0;
	}
	
	unsigned long int nPos = 0;
	for( const char *s = k; *s; s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;
}

template<> bool Bu::__cmpHashKeys<const char *>( const char * const &a, const char * const &b )
{
	if( a == b )
		return true;

	for(int j=0; a[j] == b[j]; j++ )
		if( a[j] == '\0' )
			return true;

	return false;
}

template<>
uint32_t Bu::__calcHashCode<char *>( char * const &k )
{
	if (k == NULL)
	{
		return 0;
	}
	
	unsigned long int nPos = 0;
	for( const char *s = k; *s; s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;
}

template<> bool Bu::__cmpHashKeys<char *>( char * const &a, char * const &b )
{
	if( a == b )
		return true;

	for(int j=0; a[j] == b[j]; j++ )
		if( a[j] == '\0' )
			return true;

	return false;
}

template<> uint32_t Bu::__calcHashCode<std::string>( const std::string &k )
{
	std::string::size_type j, sz = k.size();
	const char *s = k.c_str();

	unsigned long int nPos = 0;
	for( j = 0; j < sz; j++, s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;
}

template<> bool Bu::__cmpHashKeys<std::string>( const std::string &a, const std::string &b )
{
	return a == b;
}

