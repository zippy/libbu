/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ITO_HEAP_H
#define BU_ITO_HEAP_H

#include "bu/heap.h"
#include "bu/itomutex.h"
#include "bu/itocondition.h"

namespace Bu
{
	class ItoMutex;
	class ItoCondition;

	template<typename item, typename cmpfunc=__basicLTCmp<item>,
		typename itemalloc=std::allocator<item> >
	class ItoHeap
	{
	public:
		ItoHeap()
		{
		}

		virtual ~ItoHeap()
		{
		}

		void enqueue( item i )
		{
			imData.lock();
			hData.enqueue( i );
			icBlock.signal();
			imData.unlock();
		}

		item dequeue( bool bBlock=false )
		{
			imData.lock();
			if( hData.isEmpty() )
			{
				imData.unlock();

				if( bBlock )
				{
					icBlock.lock();

					while( hData.isEmpty() )
						icBlock.wait();

					imData.lock();
					try
					{
						item iRet = hData.dequeue();
						imData.unlock();
						icBlock.unlock();
						return iRet;
					}
					catch(...)
					{
						imData.unlock();
						icBlock.unlock();
						throw;
					}
				}
				throw HeapException("Heap empty.");
			}
			else
			{
				try
				{
					item iRet = hData.dequeue();
					imData.unlock();
					return iRet;
				}
				catch(...)
				{
					imData.unlock();
					throw;
				}
			}
		}

		item dequeue( int iSec, int iUSec )
		{
			imData.lock();
			if( hData.isEmpty() )
			{
				imData.unlock();

				icBlock.lock();

				icBlock.wait( iSec, iUSec );

				imData.lock();
				try
				{
					item iRet = hData.dequeue();
					imData.unlock();
					icBlock.unlock();
					return iRet;
				}
				catch(...)
				{
					imData.unlock();
					icBlock.unlock();
					throw;
				}
			}
			else
			{
				try
				{
					item iRet = hData.dequeue();
					imData.unlock();
					return iRet;
				}
				catch(...)
				{
					imData.unlock();
					throw;
				}
			}
		}
		
		bool isEmpty()
		{
			imData.lock();
			bool bRet = hData.isEmpty();
			imData.unlock();
			return bRet;
		}

		int getSize()
		{
			imData.lock();
			int iRet = hData.getSize();
			imData.unlock();
			return iRet;
		}

	private:
		Heap< item, cmpfunc, itemalloc > hData;
		ItoMutex imData;
		ItoCondition icBlock;
	};
};

#endif

