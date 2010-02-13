/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_F_STRING_H
#define BU_F_STRING_H

#include "bu/fbasicstring.h"

namespace Bu
{
	typedef FBasicString<char> FString;

	template<typename T>
	uint32_t __calcHashCode( const T &k );

	template<typename T>
	bool __cmpHashKeys( const T &a, const T &b );

	template<> uint32_t __calcHashCode<FString>( const FString &k );
	template<> bool __cmpHashKeys<FString>(
		const FString &a, const FString &b );
	
	template<typename t> void __tracer_format( const t &v );
	template<> void __tracer_format<FString>( const FString &v );

	bool &operator<<( bool &dst, const FString &sIn );
	uint8_t &operator<<( uint8_t &dst, const FString &sIn );
	int8_t &operator<<( int8_t &dst, const FString &sIn );
	char &operator<<( char &dst, const FString &sIn );
	uint16_t &operator<<( uint16_t &dst, const FString &sIn );
	int16_t &operator<<( int16_t &dst, const FString &sIn );
	uint32_t &operator<<( uint32_t &dst, const FString &sIn );
	int32_t &operator<<( int32_t &dst, const FString &sIn );
	uint64_t &operator<<( uint64_t &dst, const FString &sIn );
	int64_t &operator<<( int64_t &dst, const FString &sIn );
	float &operator<<( float &dst, const FString &sIn );
	double &operator<<( double &dst, const FString &sIn );
	long double &operator<<( long double &dst, const FString &sIn );
	Bu::FString &operator<<( Bu::FString &dst, const FString &sIn );

}

/***** I dunno about this block, I don't really want to have it... *****
#include <ostream>
std::basic_ostream<char>& operator<< (std::basic_ostream<char> &os,
	const Bu::FString &val );
*/

#endif
