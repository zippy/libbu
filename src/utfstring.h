#ifndef BU_UTF_STRING_H
#define BU_UTF_STRING_H

#include "bu/fbasicstring.h"

namespace Bu
{
	typedef FBasicString<short> UtfString;

	template<typename T>
	uint32_t __calcHashCode( const T &k );

	template<typename T>
	bool __cmpHashKeys( const T &a, const T &b );

	template<> uint32_t __calcHashCode<UtfString>( const UtfString &k );
	template<> bool __cmpHashKeys<UtfString>(
		const UtfString &a, const UtfString &b );
	
	template<typename t> void __tracer_format( const t &v );
	template<> void __tracer_format<UtfString>( const UtfString &v );
}

#endif
