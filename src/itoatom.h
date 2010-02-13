/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_ATOM_H
#define BU_ITO_ATOM_H

#include <pthread.h>

#include "itomutex.h"
#include "itocondition.h"

namespace Bu
{
	/**
	 * A thread-safe wrapper class.
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
		
		~ItoAtom()
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

		ItoMutex mOperate;	/**< The master mutex, used on all operations. */
	};
};

#endif
