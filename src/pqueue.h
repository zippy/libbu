#ifndef PQUEUE_H
#define PQUEUE_H

#include "queue.h"

/** Priority queue.  This is just like a queue, but something with a higher
  * priority will always come off the queue before something with a lower
  * priority, even if it's added after.  Otherwise works just like a queue.
  *@author Mike Buland
  */
class PQueue
{
public:
	/** Create a queue with any number of different priorities.
	  *@param nNewNumQueues The number of queues, the default is 3
	  */
	PQueue( int nNewNumQueues=3 );

	/**
	 * Cleanup all contained queues.
	 */
	~PQueue();

	/** Add a new item to the queue at the specified priority.  A lower
	  * number means a higher priority!
	  *@param pData A pointer to the data to add to the queue
	  *@param nQueueLevel The priority to set the new data to
	  */
	void enqueue( void *pData, int nQueueLevel );
	
	/** Pull the next item off the queue, high priority first.
	  *@returns A pointer to the data that was next in the priority queue
	  */
	void *dequeue();

private:
	/**
	 * The queues we use for real data storage.
	 */
	Queue *aQueue;

	/**
	 * The number of priorities or queus that we need.
	 */
	int nNumQueues;
};

#endif
