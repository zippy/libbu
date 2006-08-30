#include "hashfunctionstring.h"
#ifndef NULL
#define NULL ((void *) 0)
#endif

HashFunctionString::HashFunctionString()
{
}

HashFunctionString::~HashFunctionString()
{
}

unsigned long int HashFunctionString::hash( const void *id )
{
	if (id == NULL)
	{
		return 0;
	}
	
	unsigned long int nPos = 0;
	for( const char *s = (const char *)id; *s; s++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}
	return nPos;
}

bool HashFunctionString::cmpIDs( const void *id1, const void *id2 )
{
	if (id1 == NULL || id2 == NULL)
	{
		return false;
	}
	if (id1 == id2)
	{
		return true;
	}
	
	const char *str1 = (const char *)id1;
	const char *str2 = (const char *)id2;

	int j;
	for( j = 0; str1[j] != '\0' && str2[j] != '\0'; j++ )
	{
		if( str1[j] != str2[j] )
			return false;
	}
	return (str1[j]==str2[j]);
}

