#include "bu/utfstring.h"

template class Bu::FBasicString<short>;

template<> uint32_t Bu::__calcHashCode<Bu::UtfString>( const Bu::UtfString &k )
{
	long j, sz = k.getSize()*2;
	const char *s = (const char *)k.getStr();

	long nPos = 0;
	for( j = 0; j < sz; j++, s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}

	return nPos;	
}

template<> bool Bu::__cmpHashKeys<Bu::UtfString>(
		const Bu::UtfString &a, const Bu::UtfString &b )
{
	return a == b;
}

template<> void Bu::__tracer_format<Bu::UtfString>( const Bu::UtfString &v )
{
	printf("(%ld)\"%s\"", v.getSize(), (const char *)v.getStr() );
}

