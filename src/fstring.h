/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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

	template<> uint32_t __calcHashCode<FString>( const FString &k );
	template<> bool __cmpHashKeys<FString>(
		const FString &a, const FString &b );
	
	template<typename t> void __tracer_format( const t &v );
	template<> void __tracer_format<FString>( const FString &v );
}

/***** I dunno about this block, I don't really want to have it... *****
#include <ostream>
std::basic_ostream<char>& operator<< (std::basic_ostream<char> &os,
	const Bu::FString &val );
*/

#endif
