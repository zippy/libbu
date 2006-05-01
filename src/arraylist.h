/** \file arraylist.h
 * Describes the ArrayList class.
 *@author Mike Buland
 */
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include "list.h"

/** A simple list which uses an array.  This is a great choice if you won't do
  * a lot of adding and deleting and need a fast random access list.  Otherwise
  * use the LinkedList.
  *@author Mike Buland
  */
class ArrayList : public List
{
public:
	/** Creates an arraylist with some pre-defined specs spelled out.
	  *@param initSize the inital number of elements to allocate.
	  *@param growByFactor How much to increase the size of the array by
	  * each time we run out of room.
	  */
	ArrayList( int initSize=100, int growByFactor=10 );
	/**
	 * Destroy the ArrayList
	 */
	~ArrayList();

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
	 * Checks to see if the system needs to be resized, if it does, this will
	 * automatically resize based on your parameters.
	 */
	void checkResize();

	/**
	 * Resize the system to a specified size.  If it is larger, then all data
	 * will be retained, if smaller the elements at the end will be cut off.
	 *@param newSize The number of elements to include after resizing.
	 */
	void resizeTo( int newSize );

	/**
	 * Actual master array of pointers.  This is done to follow the List specs.
	 * All data transactions are performed with pointers or compatable
	 * primitive data-types.
	 */
	void **apData;

	/**
	 * The number of filled in elements in the array.  This is the practical
	 * real size of the ArrayList for all userspace applications.
	 */
	int nSize;

	/**
	 * The number of elements allocated in memory.  Not all of these have to be
	 * filled in, and it is usually larger than nSize so that adding and
	 * deleting elements is fast and easy.
	 */
	int nCapacity;

	/**
	 * The amount to grow by whenever the array needs resizing.
	 */
	int nGrowByFactor;
};

#endif

