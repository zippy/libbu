#ifndef HASH_FUNCTION
#define HASH_FUNCTION

/** This represents the shell of a hash function.  It must be aggregated in
  * order to be used.  Please read about it's two functions for specificatins
  * relating to what values will be passed to them and what they should return
  * for creating your own hash functions.
  *@author Mike Buland.
  */
class HashFunction
{
public:
	/**
	 * Standard Constructor.
	 */
	HashFunction();

	/**
	 * Standard Deconstructor.
	 */
	virtual ~HashFunction();

	/** Hashes the value represnted by id.  This must return a fairly unique
	  * number in the range of 0-2^32 (or whatever the size of an unsigned long
	  * is on your system) based on the id given.  The faster the number changes
	  * the better in a general sence.  The return value will be the index
	  * (after probing takes place) to the data assosiated with an id, so this
	  * function should always produce the same number for any given id.
	  *@param id The identifier to use to create a unique numerical identifier.
	  *@returns A mostly unique numerical identifier generated using the given
	  * id.
	  */
	virtual unsigned long int hash( const void *id ) = 0;

	/** This function must compare two ids in the format that this hashfunction
	  * accepts.  For example, if the hash function hashes strings it should
	  * probably { return strcmp( id1, id2 ) == 0 }.
	  *@param id1 One value to use in the comparison
	  *@param id2 Another value to use in the comparison
	  *@returns True if the two values match, otherwise false.
	  */
	virtual bool cmpIDs( const void *id1, const void *id2 ) = 0;
	
//	virtual void *createPersistantID( const void *id ) = 0;
//	virtual void destroyPersistantID( const void *id ) = 0;
};

#endif
