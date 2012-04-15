/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SYNCHRO_ATOM_H
#define BU_SYNCHRO_ATOM_H

#include <pthread.h>

#include "bu/mutex.h"

namespace Bu
{
	/**
	 * A thread-safe wrapper class.
	 *@ingroup Threading
	 */
	template <class T>
	class SynchroAtom
	{
	public:
		/**
		 * Construct an empty queue.
		 */
		SynchroAtom()
		{
		}

		SynchroAtom( const T &src ) :
			data( src )
		{
		}
		
		~SynchroAtom()
		{
		}

		T get()
		{
			mOperate.lock();
			T ret = data;
			mOperate.unlock();
			return ret;
		}

		void set( const T &val )
		{
			mOperate.lock();
			data = val;
			mOperate.unlock();
		}

	private:
		T data;

		Mutex mOperate;	/**< The master mutex, used on all operations. */
	};
};

#endif
