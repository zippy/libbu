/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ARRAY_H
#define BU_ARRAY_H

#include <memory>
#include "bu/exceptionbase.h"

namespace Bu
{
	/**
	 * Array type container, just like a normal array only flexible and keeps
	 * track of your memory for you.
	 *
	 *@param value (typename) The type of data to store in your list
	 *@param valuealloc (typename) Memory Allocator for your value type
	 *@param linkalloc (typename) Memory Allocator for the list links.
	 *@ingroup Containers
	 */
	template<typename value, int inc=10, typename valuealloc=std::allocator<value> >
	class Array
	{
	private:
		typedef class Array<value, inc, valuealloc> MyType;

	public:
		Array() :
			pData( NULL ),
			iSize( 0 ),
			iCapacity( 0 )
		{
		}

		Array( const MyType &src ) :
			pData( NULL ),
			iSize( 0 ),
			iCapacity( 0 )
		{
		//	for( Link *pCur = src.pFirst; pCur; pCur = pCur->pNext )
		//	{
		//		append( *pCur->pValue );
		//	}
		}

		~Array()
		{
			clear();
		}

		/**
		 * Clear the data from the list.
		 */
		void clear()
		{
		}

		//operator 

		/**
		 * Get the current size of the list.
		 *@returns (int) The current size of the list.
		 */
		int getSize() const
		{
			return iSize;
		}

		int getCapacity() const
		{
			return iCapacity;
		}

	private:
		valuealloc va;
		value *pData;
		int iSize;
		int iCapacity;
	};
}

#endif
