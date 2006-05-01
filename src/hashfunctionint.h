#ifndef HASH_FUNCTION_INT
#define HASH_FUNCTION_INT

#include "hashfunction.h"

/** A hash function for integer data.  Really, this does almost nothing except
  * ensure we're dealing with positive indicies.
  *@author Mike Buland.
  */
class HashFunctionInt : public HashFunction
{
public:
	/**
	 * Standard Constructor.
	 */
	HashFunctionInt();
	
	/**
	 * Standard Deconstructor.
	 */
	~HashFunctionInt();
	unsigned long int hash( const void *id );
	bool cmpIDs( const void *id1, const void *id2 );
};

#endif
