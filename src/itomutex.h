/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_MUTEX_H
#define BU_ITO_MUTEX_H

#include <pthread.h>

namespace Bu
{
	/**
	 * Simple mutex wrapper.  Currently this doesn't do anything extra for you
	 * except keep all of the functionality together in an OO sorta' way and
	 * keep you from having to worry about cleaning up your mutexes properly,
	 * or initing them.
	 *@ingroup Threading
	 */
	class ItoMutex
	{
	public:
		/**
		 * Create an unlocked mutex.
		 */
		ItoMutex();
		
		/**
		 * Destroy a mutex.  This can only be done when a mutex is unlocked.
		 * Failure to unlock before destroying a mutex object could cause it to
		 * wait for the mutex to unlock, the odds of which are usually farily
		 * low at deconstruction time.
		 */
		~ItoMutex();

		/**
		 * Lock the mutex.  This causes all future calls to lock on this
		 * instance of mutex to block until the first thread that called mutex
		 * unlocks it.  At that point the next thread that called lock will get
		 * a chance to go to work.  Because of the nature of a mutex lock it is
		 * a very bad idea to do any kind of serious or rather time consuming
		 * computation within a locked section.  This can cause thread-deadlock
		 * and your program may hang.
		 */
		int lock();

		/**
		 * Unlock the mutex.  This allows the next thread that asked for a lock
		 * to lock the mutex and continue with execution.
		 */
		int unlock();

		/**
		 * Try to lock the mutex.  This is the option to go with if you cannot
		 * avoid putting lengthy operations within a locked section.  trylock
		 * will attempt to lock the mutex, if the mutex is already locked this
		 * function returns immediately with an error code.
		 */
		int trylock();

	protected:
		pthread_mutex_t mutex;	/**< The internal mutex reference. */
	};
}

#endif
