#ifndef HASH_FUNCTION_CASE_STRING
#define HASH_FUNCTION_CASE_STRING

#include "hashfunction.h"

/** A hash function for string data.  This hash function does strings, but is
  * actually generalized to handle any binary stream of characters terminated
  * by a null character.  This is different than HashFunctionString in that
  * this does comparisons without regaurd to case.
  *@author Mike Buland.
  */
class HashFunctionCaseString : public HashFunction
{
public:
	/**
	 * Standard Constructor.
	 */
	HashFunctionCaseString();
	
	/**
	 * Standard Deconstructor.
	 */
	~HashFunctionCaseString();
	unsigned long int hash( const void *id );
	bool cmpIDs( const void *id1, const void *id2 );
};

#endif
