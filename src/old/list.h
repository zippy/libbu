#ifndef LIST_H
#define LIST_H


/** The basic List class ADT.  This, on it's own, does absolutely nothing, but
  * does define all standard interface functions to access a list.
  *@author Mike Buland
  */
class List
{
public:
	/**
	 * Construct a list.
	 */
	List();

	/**
	 * Desconstruct a list.
	 */
	virtual ~List();

	/** Gets the value at a specified index.
	  *@param nIndex The index of the item to return.
	  *@returns The specified item, or NULL if the index was beyond the range
	  * of the list.
	  *@author Mike Buland
	  */
	virtual void *getAt( int nIndex ) = 0;

	/** Append the given data to the end of the list.  This increases the
	  * size of the list by one.
	  *@param pData The data to append to the list.
	  *@author Mike Buland
	  */
	virtual void append( void *pData ) = 0;

	/** Inserts an item at the specified position in the list.  The
	  * new item takes the index that you specify, and all other items
	  * are moved up one position.  The size of the list is increased by
	  * one.
	  *@param pData The value to insert into the list.
	  *@param nPos Where to insert the data into the list.
	  *@author Mike Buland
	  */
	virtual void insertBefore( void *pData, int nPos = 0 ) = 0;

	/** Determines the size of the list, in elements.
	  *@returns The size of the list.
	  *@author Mike Buland
	  */
	virtual int getSize(  ) = 0;

	/** Determines if the list is empty or not.
	  *@returns True if the list is empty, or false if the list has
	  * data in it (if the size is greater than zero).
	  *@author Mike Buland
	  */
	virtual bool isEmpty(  ) = 0;

	/** Deletes an item at the specified index and moves all other
	  * values down one index.  The size of the list is decreased by one.
	  *@param nIndex The index of the item to delete.
	  *@author Mike Buland
	  */
	virtual void deleteAt( int nIndex ) = 0;

	/** Completely empties the list, and sets the effective size to
	  * zero.
	  *@author Mike Buland
	  */
	virtual void empty() = 0;

	/** Sets the size of the list.  This can be larger or smaller
	  * than what it was previously.  If larger, new blank items will
	  * be added to the end of the list.  If smaller than the old list
	  * items will be deleted from the end.
	  *@param nNewSize The new size of the list.
	  *@author Mike Buland
	  */
	virtual void setSize( int nNewSize ) = 0;

	/** Sets a member at a specified location to a new value.
	  * If the member being set is outside of the range of the
	  * current list it should be expanded.
	  *@param nIndex The zero-based index of the item to change.
	  *@param pData The new value for that index.
	  *@author Mike Buland
	  */
	virtual void setAt( int nIndex, void *pData ) = 0;

	/** Makes the List work like an array.  Just say listObj[2] to get
	  * the third element.
	  *@param nIndex The index to access in the list.
	  *@returns A pointer to the data at element index.
	  *@author Mike Buland
	  */
	void *operator[]( int nIndex ) { return getAt( nIndex ); };
};

#endif

