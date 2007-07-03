#include "queue.h"

void Queue::enqueue( void *data )
{
	lQueueData.append( data );
}

void *Queue::dequeue()
{
	void *dat = lQueueData[0];
	if( dat != NULL )
	{
		lQueueData.deleteAt( 0 );
	}
	return dat;
}

bool Queue::isEmpty()
{
	return lQueueData.isEmpty();
}

void Queue::empty()
{
	lQueueData.empty();
}
