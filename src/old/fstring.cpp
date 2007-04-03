#include "fstring.h"
#include "hash.h"

template<> uint32_t __calcHashCode<FString>( const FString &k )
{
	return __calcHashCode( k.c_str() );
}

template<> bool __cmpHashKeys<FString>( const FString &a, const FString &b )
{
	return a == b;
}

