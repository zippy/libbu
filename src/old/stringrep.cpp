#include "stringrep.h"

int32_t stringlen( const char *s )
{
	for( int32_t i = 0;; i++ )
		if( !s[i] )
			return i;
}

char *stringdup( const char *s )
{
	int len = stringlen( s );
	char *r = new char[len+1];
	for( int j = 0; j <= len; j++ )
		r[j] = s[j];

	return r;
}

