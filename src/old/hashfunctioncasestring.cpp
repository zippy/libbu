#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashfunctioncasestring.h"

HashFunctionCaseString::HashFunctionCaseString()
{
}

HashFunctionCaseString::~HashFunctionCaseString()
{
}

unsigned long int HashFunctionCaseString::hash( const void *id )
{
	const char *str = (const char *)id;
	unsigned long int nPos = 0;
	for( int j = 0; str[j] != '\0'; j++ )
	{
		nPos = tolower(str[j]) + (nPos << 6) + (nPos << 16) - nPos;
//		nPos += nPos<<16|(((unsigned long int)tolower(str[j]))<<((j*7)%24));
	}
	return nPos;
}

bool HashFunctionCaseString::cmpIDs( const void *id1, const void *id2 )
{
	const char *str1 = (const char *)id1;
	const char *str2 = (const char *)id2;

	int j;
	for( j = 0; str1[j] != '\0' && str2[j] != '\0'; j++ )
	{
		if( tolower(str1[j]) != tolower(str2[j]) )
			return false;
	}
	return (str1[j]==str2[j]);
}

