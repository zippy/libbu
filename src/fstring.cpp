#include "fstring.h"
#include "hash.h"

template<> uint32_t Bu::__calcHashCode<Bu::FString>( const Bu::FString &k )
{
	return __calcHashCode( k.c_str() );
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

