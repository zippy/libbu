#include "pqueue.h"

PQueue::PQueue( int nNewNumQueues )
{
	nNumQueues = nNewNumQueues;
	aQueue = new Queue[nNumQueues];
}

PQueue::~PQueue()
{
	delete[] aQueue;
}

void PQueue::enqueue( void *pData, int nQueueLevel )
{
	if( nQueueLevel < 0 || nQueueLevel >= nNumQueues )
		return;

	aQueue[nQueueLevel].enqueue( pData );
}

void *PQueue::dequeue()
{
	for( int j = 0; j < nNumQueues; j++ )
	{
		if( aQueue[j].isEmpty() == false )
		{
			return aQueue[j].dequeue();
		}
	}

	return NULL;
}
