#ifndef HASH_FUNCTION_STRING
#define HASH_FUNCTION_STRING

#include "hashfunction.h"

/** A hash function for string data.  This hash function does strings, but is
  * actually generalized to handle any binary stream of characters terminated
  * by a null character.
  *@author Mike Buland.
  */
class HashFunctionString : public HashFunction
{
public:
	/**
	 * Standard Constructor.
	 */
	HashFunctionString();
	
	/**
	 * Standard Deconstructor.
	 */
	~HashFunctionString();
	unsigned long int hash( const void *id );
	bool cmpIDs( const void *id1, const void *id2 );
};

#endif
