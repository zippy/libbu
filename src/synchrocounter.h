/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SYNCHRO_COUNTER_H
#define BU_SYNCHRO_COUNTER_H

#include "bu/mutex.h"

namespace Bu
{
	/**
	 * A simple thread-safe counter class.  This is handy for assigning unique
	 * IDs to objects that are being created in different threads.
	 *@ingroup Threading Containers
	 */
	template <class T>
	class SynchroCounter
	{
	public:
		SynchroCounter() :
			tCounter( 0 )
		{
		}
		
		virtual ~SynchroCounter()
		{
		}

		T next()
		{
			mOperate.lock();
			T tRet = tCounter;
			tCounter++;
			mOperate.unlock();

			return tRet;
		}

	private:
		T tCounter;				/**< The counter itself. */
		Mutex mOperate;	/**< The master mutex, used on all operations. */
	};
}

#endif
