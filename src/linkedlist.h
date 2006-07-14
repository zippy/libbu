/**@file
 * Describes the LinkedList implementation of the List ADT.
 *@author Mike Buland
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include "list.h"

/** A linked-item implementation of the List ADT.  Since the data is linked
  * sequentially this is a great choice for lists that will grow and shrink
  * a lot, but don't require as much random access.  This implementation
  * includes optomizations that make iterating through data, and appending
  * items to the list take O(1) time.
  *@author Mike Buland
  */
class LinkedList : public List
{
public:
	/**
	 * Construct a blank LinkedList.
	 */
    LinkedList();

	/**
	 * Delete all list data, but do not delete any of the contained elements.
	 */
	virtual ~LinkedList();

    void *getAt( int nIndex );
    void append( void *pData );
    void insertBefore( void *pData, int nPos = 0 );
    int getSize(  );
    bool isEmpty(  );
	void deleteAt( int nIndex );
	void empty();
	void setSize( int nNewSize );
	void setAt( int nIndex, void *pData );

private:
	/**
	 * A link in the linked list.
	 */
    class Link
    {
      public:
		/**
		 * Construct an empty link.
		 */
        Link()
        {
            pData = NULL;
            pNext = NULL;
        }
		/**
		 * Construct a link filled in with useful data.
		 *@param newData The data this link should hold.
		 *@param newNext The next link that this link should point to.
		 */
        Link( void *newData = NULL, Link * newNext = NULL )
        {
            pData = newData;
            pNext = newNext;
        }
        void *pData;	/**< A pointer to the contained data. */
        Link *pNext;	/**< A pointer to the next link in the chain */
    };

	/**
	 * Finds a pointer to the link at index index.  This is the core function
	 * called for all seek operations, and has been optimized as heavily as
	 * possible.
	 *@param index The zero-based index of the desired element.
	 *@returns A pointer to the requested Link, or NULL if it isn't found.
	 */
	Link *getPtrTo( int index );
    Link *pBase;	/**< The first link in the list. */
	Link *pTop;		/**< The Last link in the list. */
    Link *pLast;	/**< The previously requested link. */
    int nSize;		/**< The number of contained links. */
    int nLast;		/**< The index of the previously requested link. */
};

#endif

