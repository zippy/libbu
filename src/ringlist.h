#ifndef RINGLIST_H
#define RINGLIST_H

#include "list.h"

/**
 * A RingList or Ring Buffer implementation.  This is a list that never grows in
 * size once it is created, but instead once it is full new items added to the
 * RingList replace the oldest items and the zero-index is virtually shifted.
 * Since no data is actually moved when zero-index moves, this is very
 * efficient.
 * <br>
 * The items removed are not actually deleted by the RingList, so instead they
 * are first moved into a temporary "Push Buffer" that can be accessed so that
 * elements pushed off the edge of the RingList can be accessed for cleanup.
 *@author Mike Buland
 */
class RingList : public List
{
public:
	/**
	 * Construct a RingList with a fixed initial size.  This size never changes
	 * unless setSize is called later during normal operation.
	 *@param nInitSize The number of elements to allocate.
	 */
	RingList( int nInitSize );

	/**
	 * Clean up the data structures, but not the contained elements.
	 */
	~RingList();
	
	/**
	 * Get an element at the specified index.
	 *@param nIndex The index of the element to retreive.
	 *@returns A pointer to the requested element, or NULL if the element is
	 * not found or not initialized yet.
	 */
	void *getAt( int nIndex );

	/**
	 * Append an element to the end of the list, overwriting the begining if
	 * necesarry.
	 *@param pData The pointer to append to the RingList.
	 */
	void append( void *pData );

	/**
	 * Insert an element before another in the RingList, pushing all after it
	 * down the list.
	 *@param pData The data to insert.
	 *@param nPos The position that new the element should occupy in the list.
	 */
	void insertBefore( void *pData, int nPos = 0 );

	/**
	 * Get the size of the array.
	 */
	int getSize();

	/**
	 * Is the RingList empty?
	 *@returns True if it is empty, false otherwise.
	 */
	bool isEmpty();

	/**
	 * Delete an element in the list, moving all later elements down one index.
	 *@param nIndex The index of the element to delete.
	 */
	void deleteAt( int nIndex );

	/**
	 * Remove all elements from the RingList.
	 */
	void empty();

	/**
	 * Set a new size for the RingList.  Be careful with this one, if shrinking
	 * this may quietly create a memory leak.
	 *@param nNewSize The new size to set the RingList to.
	 *@todo Either fix this memory leak somehow or remove this function.
	 */
	void setSize( int nNewSize );
	
	/**
	 * Set a specific element to a new value.
	 *@param nIndex The zero-based index to change the value of.
	 *@param pData The data to put at the location specified by nIndex.
	 */
	void setAt( int nIndex, void *pData );

	/**
	 * Retrieve the contents of the push buffer.  This is the data that is
	 * pushed off the end of the array if you append data and the list is full.
	 * This should be checked after every append operation to be sure there
	 * isn't anything that needs deleting.
	 *@returns The last value pushed off the RingList, or NULL if nothing was
	 * pushed off.
	 */
	void *getPushBuf();
	
private:
	int nFirstIndex;	/**< The index to be translated as zero. */
	int nRealLength;	/**< The Amount of storage space available. */
	int nDataLength;	/**< The number of elements filled in. */
	void **apData;		/**< The actual data storage. */
	void *pPushBuf;		/**< The push buffer. */

};

#endif
