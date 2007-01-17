#include "hash.h"

subExceptionDef( HashException )

template<> uint32_t __calcHashCode<const int>( const int k )
{
	return k;
}

template<> bool __cmpHashKeys<const int>( const int a, const int b )
{
	return a == b;
}

template<> uint32_t __calcHashCode<int>( int k )
{
	return k;
}

template<> bool __cmpHashKeys<int>( int a, int b )
{
	return a == b;
}

template<> uint32_t __calcHashCode<const unsigned int>( const unsigned int k )
{
	return k;
}

template<> bool __cmpHashKeys<const unsigned int>( const unsigned int a, const unsigned int b )
{
	return a == b;
}

template<> uint32_t __calcHashCode<unsigned int>( unsigned int k )
{
	return k;
}

template<> bool __cmpHashKeys<unsigned int>( unsigned int a, unsigned int b )
{
	return a == b;
}

template<>
uint32_t __calcHashCode<const char *>( const char * k )
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

template<> bool __cmpHashKeys<const char *>( const char *a, const char *b )
{
	if( a == b )
		return true;

	for(; *a == *b; a++, b++ )
		if( *a == '\0' )
			return true;

	return false;
}

template<>
uint32_t __calcHashCode<char *>( char *k )
{
	return __calcHashCode<const char *>((const char *)k );
}

template<> bool __cmpHashKeys<char *>( char *a, char *b )
{
	return __cmpHashKeys<const char *>((const char *)a, (const char *)b );
}

template<> uint32_t __calcHashCode<const std::string>( const std::string k )
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

template<> bool __cmpHashKeys<const std::string>( const std::string a, const std::string b )
{
	return a == b;
}

template<> uint32_t __calcHashCode<std::string>( std::string k )
{
	return __calcHashCode<const std::string>( k );
}

template<> bool __cmpHashKeys<std::string>( std::string a, std::string b )
{
	return __cmpHashKeys<const std::string>( a, b );
}

template<> uint32_t __calcHashCode<Hashable &>( Hashable &k )
{
	return k.getHashCode();
}

template<> bool __cmpHashKeys<Hashable &>( Hashable &a, Hashable &b )
{
	return a.compareForHash( b );
}

