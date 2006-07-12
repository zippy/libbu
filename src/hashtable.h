/**\hashtable.h
 * Describes the HashFunction, HashFunctionString, and HashTable classes.  It
 * was just easier to put them all in one set of files.
 *@author Mike Buland
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

//Uncomment this line to see a cool text-mode visualization of what's going on
//#define HASH_DEBUG_VIS 1

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashfunction.h"

/**
 * A simple yet flexable hash-table.  This uses several tricks to help ensure
 * that the table is always running at maximum efficiency.  You no longer have
 * to specify a "danger fill level" when more space is needed a rehash is
 * automatically trigered.  Deleting elements is fully supported, as well as
 * duplicate elements.  To work with and allow duplicates simple construct your
 * HashTable the way you normally would, but when deleting or getting elements
 * you can specify a skip value.  This effectively allows you to treat elements
 * with duplicate ID's as though they were in a zero-based array.  The first
 * element inserted with a given ID would be at skip zero, the next at skip 1
 * and so on.  This allows you to quickly search for elements with duplicate
 * names, just stop when you get a null for a skip number, i.e.
 * <pre>
 *   for( int j = 0;; j++ )
 *   {
 *       void *pData = hash.get( myID, j );
 *       if( !pData ) break;
 *       // Do something interesting with pData
 *   }
 * </pre>
 * There are new features in this HashTable that also allow for memory saving
 * when dealing with systems where many elements are being deleted from the
 * table.  In those cases the elements deleted cannot be simply deleted, instead
 * they have to be marked as deleted and hidden from the user, but maintained in
 * the table so that future hashing operations don't fail.  When rehashing
 * occurs all elements marked as deleted are quietly removed.  In these cases,
 * if the number of deleted items would free enough space in the table for the
 * table to be used efficiently without resizing, it is left the same size and
 * rehashing is performed effectively in place, allowing the deleted items to
 * be removed.
 * <br>
 * For info on adding new hashing algorithms, please see the HashFunction class.
 *@author Mike Buland
 *@todo Fix probing for tables that allow duplicates, and delete an item, then
 * insert an item with the same name.
 */
class HashTable
{
public:
	/** Constructs a hash table.
	  *@param hNewFunc A pointer to a hashfunction class to use.  If this is
	  * null the default general string type will be used.
	  *@param nInitSize The initial size of the hashtable.
	  *@param bAllowDupes Setting this value to true allows the system to
	  * insert more than one copy of any given key.  This can be tricky, and
	  * will require you to use the nSkip parameter on the get function.
	  */
	HashTable( HashFunction *hNewFunc, unsigned long int nInitSize, bool bAllowDupes=false );
	
	/**
	 * Destroys the hashtable, cleaning up all internal storage, but not stored
	 * elements.  Also deletes the HashFunction passed in in the constructor.
	 */
	~HashTable();

	/** Inserts an item into the hashtable.  This function will trigger a
	  * rehash if adding another item would force the table's load factor over
	  * the danger level.
	  *@param id used to find the data later.
	  *@param data The data item to insert into the table with the identifier
	  * id
	  *@returns True if insertion was successfull, and false if it failed.
	  */
	bool insert( const void *id, const void *data );

	/** Gets an item in the hashtable based on the id of that item.  If there
	  * is more than one item with the same id you can use the nSkip parameter
	  * to access all of them.
	  *@param id The id of the item you're trying to find.
	  *@param nSkip The number of items with that id to skip before returning
	  * with the requested item.
	  *@returns A pointer to the data stored at the given id.
	  */
	const void *get( const void *id, unsigned long int nSkip=0 );

	/** Gets the total capacity of the hashtable.  This is actually the number
	  * of total positions available inside the hashtable at the moment.  This
	  * will change when the hashtable's load exceeds the danger level.
	  * Please note that this is NOT the actual amount of space available.
	  * In reality you can only access about 45-50 percent of that space.
	  *@returns The total capacity.
	  */
	unsigned long int getCapacity();

	/** Gets the number of filled in items in the hash table.  This is roughly
	  * equivelent to the getSize function assosiated with the Lists.
	  *@returns The number of filled in items in the hash table.
	  */
	unsigned long int getSize();

	/** Gets the load (percentage) of filled in items in the table.  This is
	  * technically the size divided by the capacity, but is definately usefull
	  * since it's required to check if it's time to rehash.
	  *@returns The table load in the range 0.0 to 1.0
	  */
	double getLoad();

	/** Sets up an xPos object for use indexing the items in the table.  Call
	  * this first and follow the directions for getNextItemPos below to
	  * iterate through every item in the table, while avoiding the empty
	  * spaces.
	  *@returns A pointer to a xPos object telling the hashtable where to find
	  * the item you're looking at.
	  */
	void *getFirstItemPos();

	/** Get the item's data that is being pointed to by xPos.  This is only
	  * valid after xPos was created using getFirstItemPos and getNextItemPos
	  * was called at least once.
	  *@param xPos supplied by getFirstItemPos.
	  *@returns The key value that was used to insert the data into the table.
	  */
	const void *getItemData( void *xPos );

	/** Get the item's ID that is being pointed to by xPos.  This is only
	  * valid after xPos was created using getFirstItemPos and getNextItemPos
	  * was called at least once.
	  *@param xPos supplied by getFirstItemPos.
	  *@returns The key value that was used to insert the data into the table.
	  */
	const void *getItemID( void *xPos );

	/** Used for iterating through a hash table sequentially.  This will
	  * update the xPos pointer to point to the next time, all ready to
	  * be accessed with getItemID and getItemData.  This must be called at
	  * least once before xPos is meaningful, and will return a NULL when it
	  * has reached the last item.
	  *@param xPos This must be an object created by a call to the function
	  * getFirstItemPos, and is only meaningful to the internal routines.
	  * Aborting a call in the middle (not running to the end of the table)
	  * may result in a memory leak at the moment.
	  *@returns xPos if still iterating through the list, otherwise it will
	  * return NULL when the end has been reached and the xPos variable has
	  * been deleted.
	  */
	void *getNextItemPos( void *xPos );

	/** A helpful operator to make accessing items easier.  Please note that
	  * this simply returns a pointer to the data stored internally, and cannot
	  * be used like the STL operator to store new data, use insert for that.
	  *@param id The identifier used to store the requested item.
	  *@returns The data value assosiated with the given id, or NULL if it
	  * wasn't found in the table.
	  */
	const void *operator[](const void *id);

	/**
	 * Delete the specified item from the hashtable.  This actually keeps the
	 * data and marks it deleted.  For all intents and purposes to the user it
	 * is deleted, except that the space is still used until a rehash is forced.
	 * This means that in hashtables where elements are being inserted and
	 * deleted frequently you may run into a higher rate of expansion.
	 *@param id The ID to delete.
	 *@param nSkip The number of similar id's to skip before deleting in a
	 * hashtable that allows duplicates.
	 *@returns True if the element was found and deleted, false otherwise.
	 */
	bool del( const void *id, int nSkip=0 );

	/**
	 * Deletes every entry in the hash table.  See the notes on del to see what
	 * this means, except that since no data is being kept, the entire table is
	 * just marked as usable space.
	 */
	void clear();

private:
	/**
	 * Contains info related to a position in the hashtable.  Used for
	 * searching through hashtables one item at a time, in order.  This class
	 * should never be created by anything but a HashTable, and should never
	 * be referenced directly.  Instead the hashtable returns a void pointer,
	 * which is what should be passed back in next time you use a search
	 * function.  Always finish a search, since the object is deleted at the
	 * end of the search.
	 *@author Mike Buland
	 */
	class HashPos
	{
	public:
		/** Create a blank HashPos. */
		HashPos() { bStarted=false; nPos = 0; };
		/** Has the search been started?  */
		bool bStarted;
		/** The position (index) into the backend storage structure. */
		unsigned long int nPos;
	};

	/**
	 * All data related to a single element in the hashtable.  This should
	 * really only be used and manipulated by the HashTable itself.
	 *@author Mike Buland
	 */
	typedef struct HashNode
	{
	public:
		/** Create a new, empty HashNode. */
		HashNode() { id = NULL; data = NULL; bDeleted = false; };
		/** A pointer to the original ID that was used to key the data. */
		const void *id;
		/** A pointer to the data stored along with the above ID. */
		const void *data;
		/** Weather or not this data should really...exist */
		bool bDeleted;
	} HashNode;

private:
	/**
	 * Just sets the values in the element to some friendly values.
	 *@param newID The new ID to store.
	 *@param newData The new Data to store.
	 */
	void set( int j, const void *newID, const void *newData );
	/**
	 * Tells you if the node is filled or not.
	 *@returns True=an ID has been stored here, False=no ID.
	 */
	bool isFilled( int j );
	/**
	 * This actually resizes, but since every resize requires a reHash to go
	 * along with it, that's the name.  This actually creates a new buffer for
	 * all of the contained data and then pulls every old element that was in
	 * the old table out and performs the hashing placement calculations again.
	 * This function skips all data that was marked as deleted, so at this
	 * point it really will be.
	 *@param nNewSize The new size to set the table to while re-hashing.
	 *@returns True if the operation was successful, false otherwise.
	 */
	bool reHash( unsigned long int nNewSize );

	/**
	 * Helper function to allocate a new table.  Really just does the memory
	 * allocation.
	 *@param nNewSize The size of the table to generate.
	 *@returns A new, blank array of HashNode objects the size you specified.
	 */
	HashNode *newTable( unsigned long int nNewSize );

	/**
	 * This function is used once an actual hash code is obtained.  nStart is
	 * the given hash code, which is then wrapped to the size of the table.  If
	 * there is data at that location, tests are performed to see if it's the
	 * right one.  If it is, then it is returned, otherwise a series of further
	 * tests based on a 2^n search pattern is performed.  The position of the
	 * requested data in the back-end storage is returned if found, otherwise
	 * another less useful value is returned...
	 *@param nStart The initial hashcode of the ID testing for.
	 *@param id A pointer to the id that is being searched for.
	 *@returns The real location of the data requested.
	 */
	unsigned long int probe( unsigned long int nStart, const void *id );

	/**
	 * Simple helper function to determine if a number is prime or not.
	 * This function runs in sqrt(n) time.
	 *@param num Number to test for prime-hood.
	 *@returns True if the number is prime, false otherwise.
	 */
	bool isPrime( int num );

	/**
	 * Given any number, this function finds the first number after it that is
	 * prime.  Since this number is a multiple internally it's rare that the
	 * starting number would be prime.
	 *@param base The number to start the prime search on.
	 *@returns The first prime after the number given.
	 */
	int nextPrime( int base );
	
#ifdef HASH_DEBUG_VIS
	void printDebugLine( const char *exData );
#endif

	/** A pointer to the HashFunction subclass instance to use. */
	HashFunction *hFunc;
	/** The complete array of HashNode objects to store data in. */
	HashNode *aTable;
	/** The actual size of the table, not how many elements are in it. */
	unsigned long int nTableSize;
	/** The number of elements that are in the table. */
	unsigned long int nSize;
	/** The number of elements that are unavailable now. */
	unsigned long int nFilled;
	/** Allow duplicate ID's in the table. */
	bool bAllowDupes;
};

#endif
