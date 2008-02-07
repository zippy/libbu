/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_QUEUE_H
#define BU_ITO_QUEUE_H

#include <pthread.h>

#include "itomutex.h"
#include "itocondition.h"

/**
 * A thread-safe wrapper class.
 *@author Mike Buland
 *@ingroup Threading
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
