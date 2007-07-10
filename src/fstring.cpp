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

