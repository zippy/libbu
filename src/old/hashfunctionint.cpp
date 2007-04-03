#include "hashfunctionint.h"

HashFunctionInt::HashFunctionInt()
{
}

HashFunctionInt::~HashFunctionInt()
{
}

unsigned long int HashFunctionInt::hash( const void *id )
{
	return (unsigned long)(id);
}

bool HashFunctionInt::cmpIDs( const void *id1, const void *id2 )
{
	return (unsigned long)(id1) == (unsigned long)(id2);
}

