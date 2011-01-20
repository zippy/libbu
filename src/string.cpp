/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#define BU_TRACE
#include "bu/trace.h"

#include "bu/string.h"
#include "bu/hash.h"

template class Bu::BasicString<char>;

template<> uint32_t Bu::__calcHashCode<Bu::String>( const Bu::String &k )
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

template<> bool Bu::__cmpHashKeys<Bu::String>(
		const Bu::String &a, const Bu::String &b )
{
	return a == b;
}

template<> void Bu::__tracer_format<Bu::String>( const Bu::String &v )
{
	printf("(%ld)\"%s\"", v.getSize(), v.getStr() );
}

bool &Bu::operator<<( bool &dst, const Bu::String &sIn )
{
	if( sIn == "true" || sIn == "yes" || sIn == "t" )
		dst = true;
	else
		dst = false;

	return dst;
}

uint8_t &Bu::operator<<( uint8_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhu", &dst );
	return dst;
}

int8_t &Bu::operator<<( int8_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhd", &dst );
	return dst;
}

char &Bu::operator<<( char &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hhd", &dst );
	return dst;
}

uint16_t &Bu::operator<<( uint16_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hu", &dst );
	return dst;
}

int16_t &Bu::operator<<( int16_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%hd", &dst );
	return dst;
}

uint32_t &Bu::operator<<( uint32_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%u", &dst );
	return dst;
}

int32_t &Bu::operator<<( int32_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%d", &dst );
	return dst;
}

uint64_t &Bu::operator<<( uint64_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%llu", &dst );
	return dst;
}

int64_t &Bu::operator<<( int64_t &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%lld", &dst );
	return dst;
}

float &Bu::operator<<( float &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%f", &dst );
	return dst;
}

double &Bu::operator<<( double &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%lf", &dst );
	return dst;
}

long double &Bu::operator<<( long double &dst, const Bu::String &sIn )
{
	sscanf( sIn.getStr(), "%Lf", &dst );
	return dst;
}

Bu::String &Bu::operator<<( Bu::String &dst, const Bu::String &sIn )
{
	dst = sIn;
	return dst;
}

