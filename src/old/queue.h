#ifndef QUEUE_H
#define QUEUE_H
#include "linkedlist.h"

/**
 * An ultra-simple queue implementation.  It just uses a linked list as it's
 * container so we don't have to worry about anything!
 *@author Mike Buland
 */
class Queue
{
public:
	/**
	 * Puts a new item at the end of the queue.
	 *@param data A new value to put at the end of the queue.
	 */
	void enqueue( void *data );

	/**
	 * Gets the begining item of the queue off and returns it.
	 *@returns The value at the front of the queue.
	 */
	void *dequeue();

	/**
	 * Checks if the queueu is empty.
	 *@returns True if the queueu is empty, and false if it has things in it.
	 */
	bool isEmpty();

	/**
	 * Empty the queue.
	 */
	void empty();
	
	/**
	 * Get a pointer to the internal list object.
	 *@returns A pointer to the internal list object.
	 */
	LinkedList *getList() { return &lQueueData; };

private:
	LinkedList lQueueData;	/**< Where all of the real data is stored. */
};
#endif
