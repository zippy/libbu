/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#define BU_TRACE
#include "bu/trace.h"

#include "fstring.h"
#include "hash.h"

template<> uint32_t Bu::__calcHashCode<Bu::FString>( const Bu::FString &k )
{
	long j, sz = k.getSize();
	const char *s = k.getStr();

	long nPos = 0;
	for( j = 0; j < sz; j++, s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;
}

template<> bool Bu::__cmpHashKeys<Bu::FString>(
		const Bu::FString &a, const Bu::FString &b )
{
	return a == b;
}

std::basic_ostream<char>& operator<< (std::basic_ostream<char> &os, const Bu::FString &val )
{
	os.write( val.getStr(), val.getSize() );
	return os;
}


template<> void Bu::__tracer_format<Bu::FString>( const Bu::FString &v )
{
	printf("(%ld)\"%s\"", v.getSize(), v.getStr() );
}
