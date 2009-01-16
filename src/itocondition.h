/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_CONDITION_H
#define BU_ITO_CONDITION_H

#include <pthread.h>

#include "itomutex.h"

namespace Bu
{
	/**
	 * Ito condition.  This is a fairly simple condition mechanism.  As you may
	 * notice this class inherits from the ItoMutex class, this is because all
	 * conditions must be within a locked block.  The standard usage of a
	 * condition is to pause one thread, perhaps indefinately, until another
	 * thread signals that it is alright to procede.
	 * <br>
	 * Standard usage for the thread that wants to wait is as follows:
	 * <pre>
	 * ItoCondition cond;
	 * ... // Perform setup and enter your run loop
	 * cond.lock();
	 * while( !isFinished() ) // Could be anything you're waiting for
	 *     cond.wait();
	 * ...  // Take care of what you have to.
	 * cond.unlock();
	 * </pre>
	 * The usage for the triggering thread is much simpler, when it needs to
	 * tell the others that it's time to grab some data it calls either signal
	 * or broadcast.  See both of those functions for the difference.
	 *@ingroup Threading
	 */
	class ItoCondition : public ItoMutex
	{
	public:
		/**
		 * Create a condition.
		 */
		ItoCondition();

		/**
		 * Destroy a condition.
		 */
		~ItoCondition();

		/**
		 * Wait forever, or until signalled.  This has to be called from within
		 * a locked section, i.e. before calling this this object's lock
		 * function should be called.
		 */
		int wait();

		/**
		 * Wait for a maximum of nSec seconds and nUSec micro-seconds or until
		 * signalled.  This is a little more friendly function if you want to
		 * perform other operations in the thrad loop that calls this function.
		 * Like the other wait function, this must be inside a locked section.
		 *@param nSec The seconds to wait.
		 *@param nUSec the micro-seconds to wait.
		 */
		int wait( int nSec, int nUSec );

		/**
		 * Notify the next thread waiting on this condition that they can go
		 * ahead.  This only signals one thread, the next one in the condition
		 * queue, that it is safe to procede with whatever operation was being
		 * waited on.
		 */
		int signal();

		/**
		 * Notify all threads waiting on this condition that they can go ahead
		 * now. This function is slower than signal, but more effective in
		 * certain situations where you may not know how many threads should be
		 * activated.
		 */
		int broadcast();

	private:
		pthread_cond_t cond;	/**< Internal condition reference. */
	};
}

#endif
