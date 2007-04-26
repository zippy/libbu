#ifndef ITO_QUEUE_H
#define ITO_QUEUE_H

#include <pthread.h>

#include "itomutex.h"
#include "itocondition.h"

/**
 * A thread-safe wrapper class.
 *@author Mike Buland
 */
template <class T>
class ItoAtom
{
public:
	/**
	 * Construct an empty queue.
	 */
	ItoAtom()
	{
	}

	ItoAtom( const T &src ) :
		data( src )
	{
	}
	
	~ItoQueue()
	{
	}

	T get()
	{
		mOperate.lock();
		mOperate.unlock();
		return data;
	}

	void set( const T &val )
	{
		mOperate.lock();
		data = val;
		cBlock.signal();
		mOperate.unlock();
	}

private:
	Item *pStart;	/**< The start of the queue, the next element to dequeue. */
	Item *pEnd;		/**< The end of the queue, the last element to dequeue. */

	ItoMutex mOperate;	/**< The master mutex, used on all operations. */
	ItoCondition cBlock;	/**< The condition for blocking dequeues. */
};

#endif
