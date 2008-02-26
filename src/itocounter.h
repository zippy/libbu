/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_COUNTER_H
#define BU_ITO_COUNTER_H

#include "itomutex.h"

namespace Bu
{
	/**
	 * A simple thread-safe counter class.  This is handy for assigning unique
	 * IDs to objects that are being created in different threads.
	 *@author Mike Buland
	 *@ingroup Threading Containers
	 */
	template <class T>
	class ItoCounter
	{
	public:
		ItoCounter() :
			tCounter( 0 )
		{
		}
		
		virtual ~ItoCounter()
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
		ItoMutex mOperate;	/**< The master mutex, used on all operations. */
	};
}

#endif
