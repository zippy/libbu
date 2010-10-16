/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_HEAP_H
#define BU_HEAP_H

#include <stddef.h>
#include <memory>
#include "bu/exceptionbase.h"
#include "bu/util.h"
#include "bu/queue.h"
#include "bu/sharedcore.h"

namespace Bu
{
	subExceptionDecl( HeapException );

	template<typename item, typename cmpfunc, typename itemalloc>
	class Heap;

	/** @cond DEVEL */
	template<typename item, typename cmpfunc, typename itemalloc>
	class HeapCore
	{
	friend class Heap<item, cmpfunc, itemalloc>;
	friend class SharedCore<
		Heap<item, cmpfunc, itemalloc>, HeapCore<item, cmpfunc, itemalloc>
		>;
	private:
		HeapCore() :
			iSize( 0 ),
			iFill( 0 ),
			aItem( NULL )
		{
		}

		virtual ~HeapCore()
		{
			clear();
		}

		void init()
		{
			if( iSize > 0 )
				return;

			iSize = 7;
			iFill = 0;
			aItem = ia.allocate( iSize );
		}

		void init( int iCap )
		{
			if( iSize > 0 )
				return;

			for( iSize = 1; iSize < iCap; iSize=iSize*2+1 ) { }
			iFill = 0;
			aItem = ia.allocate( iSize );
		}

		void clear()
		{
			if( iSize == 0 )
				return;

			for( int j = 0; j < iFill; j++ )
				ia.destroy( &aItem[j] );
			ia.deallocate( aItem, iSize );
			aItem = NULL;
			iSize = 0;
			iFill = 0;
		}
		
		void upSize()
		{
			if( iSize == 0 )
			{
				init();
				return;
			}

			item *aNewItems = ia.allocate( iSize*2+1 );
			//
			// We cannot use a memcopy here because we don't know what kind
			// of datastructures are being used, we have to copy them one at
			// a time.
			//
			for( int j = 0; j < iFill; j++ )
			{
				ia.construct( &aNewItems[j], aItem[j] );
				ia.destroy( &aItem[j] );
			}
			ia.deallocate( aItem, iSize );
			aItem = aNewItems;
			iSize = iSize*2+1;
		}
		
		virtual void enqueue( const item &it )
		{
			item i = it; // TODO: This is a silly workaround, put the i item
						 // at the end.
			if( iFill+1 >= iSize )
				upSize();

			for( int j = 0; j < iFill; )
			{
				if( cmp( i, aItem[j] ) )
				{
					Bu::swap( i, aItem[j] );
				}

				if( j*2+1 >= iFill )
					break;
				if( cmp( i, aItem[j*2+1] ) )
				{
					j = j*2+1;
				}
				else
				{
					j = j*2+2;
				}
			}
			ia.construct( &aItem[iFill], i );
			if( iFill > 0 )
			{
				for( int j = iFill; j >= 0; )
				{
					int k = (j-1)/2;
					if( j == k )
						break;
					if( cmp( aItem[k], aItem[j] ) )
						break;

					Bu::swap( aItem[k], aItem[j] );
					j = k;
				}
			}
			iFill++;
		}

		virtual item dequeue()
		{
			if( iFill == 0 )
				throw HeapException("Heap empty.");
			item iRet = aItem[0];
			int j;
			for( j = 0; j < iFill; )
			{
				int k = j*2+1;
				if( k+1 < iFill && cmp( aItem[k+1], aItem[k] ) )
				{
					if( k+1 < iFill-1 && cmp( aItem[iFill-1], aItem[k+1] ) )
						break;
					aItem[j] = aItem[k+1];
					j = k+1;
				}
				else if( k < iFill )
				{
					if( k < iFill-1 && cmp( aItem[iFill-1], aItem[k] ) )
						break;
					aItem[j] = aItem[k];
					j = k;
				}
				else
					break;
			}
			if( j < iFill-1 )
				aItem[j] = aItem[iFill-1];
			ia.destroy( &aItem[iFill-1] );
			iFill--;

			return iRet;
		}

	private:
		int iSize;
		int iFill;
		item *aItem;
		cmpfunc cmp;
		itemalloc ia;
	};
	/** @endcond */

	/**
	 * A priority queue that allows for an unlimited number of priorities.  All
	 * objects enqueued must support less-than-comparison.  Then every time an
	 * item is dequeued it is always the least item in the heap.  The heap
	 * operates using a binary tree for storage, which allows most operations
	 * to be very fast.  Enqueueing and dequeueing are both O(log(N)) operatoins
	 * whereas peeking is constant time.
	 *
	 * This heap implementation allows iterating, however please note that any
	 * enqueue or dequeue operation will invalidate the iterator and make it
	 * unusable (if it still works, you shouldn't trust the results).  Also,
	 * the items are not stored in memory in order, they are optomized into a
	 * tree.  This means that the items will be in effectively random order
	 * while iterating through them, and the order cannot be trusted.  Also,
	 * modifying an item in the heap will not cause that item to be re-sorted.
	 * If you want to change the position of an item in the heap you will have
	 * to dequeue every item before it, dequeue that item, change it, and
	 * re-enqueue all of the items removed.
	 */
	template<typename item, typename cmpfunc=__basicLTCmp<item>, typename itemalloc=std::allocator<item> >
	class Heap : public Queue<item>, public SharedCore<
				 Heap<item, cmpfunc, itemalloc>,
				 HeapCore<item, cmpfunc, itemalloc>
				 >
	{
	private:
		typedef class Heap<item,cmpfunc,itemalloc> MyType;
		typedef class HeapCore<item,cmpfunc,itemalloc> Core;

	protected:
		using SharedCore<MyType, Core>::core;
		using SharedCore<MyType, Core>::_hardCopy;
		using SharedCore<MyType, Core>::_resetCore;
		using SharedCore<MyType, Core>::_allocateCore;

	public:
		Heap()
		{
		}
		
		Heap( cmpfunc cmpin )
		{
			core->cmp = cmpin;
		}

		Heap( int iInitialCapacity )
		{
			core->init( iInitialCapacity );
		}
		
		Heap( cmpfunc cmpin, int iInitialCapacity )
		{
			core->cmp = cmpin;
			core->init( iInitialCapacity );
		}

		Heap( const MyType &rSrc ) :
			SharedCore<MyType, Core>( rSrc )
		{
		}

		virtual ~Heap()
		{
		}

		virtual void enqueue( const item &it )
		{
			_hardCopy();

			core->enqueue( it );
		}

		virtual item &peek()
		{
			_hardCopy();

			if( core->iFill == 0 )
				throw HeapException("Heap empty.");
			return core->aItem[0];
		}

		virtual const item &peek() const
		{
			if( core->iFill == 0 )
				throw HeapException("Heap empty.");
			return core->aItem[0];
		}

		virtual item dequeue()
		{
			_hardCopy();

			return core->dequeue();
		}

		virtual bool isEmpty() const
		{
			return (core->iFill==0);
		}

		virtual int getSize() const
		{
			return core->iFill;
		}

		class iterator
		{
			friend class const_iterator;
			friend class Heap<item, cmpfunc, itemalloc>;
		private:
			Heap<item, cmpfunc, itemalloc> *pHeap;
			int iIndex;

			iterator( Heap<item, cmpfunc, itemalloc> *pHeap, int iIndex ) :
				pHeap( pHeap ), iIndex( iIndex )
			{
			}

			void checkValid()
			{
				if( pHeap == NULL )
					throw Bu::ExceptionBase("Iterator not initialized.");
				if( iIndex < 0 || iIndex >= pHeap->core->iFill )
					throw Bu::ExceptionBase("Iterator out of bounds.");
			}

		public:
			iterator() :
				pHeap( NULL ),
				iIndex( -1 )
			{
			}

			iterator( const iterator &i ) :
				pHeap( i.pHeap ),
				iIndex( i.iIndex )
			{
			}

			bool operator==( const iterator &oth ) const
			{
				return (oth.pHeap == pHeap) && (oth.iIndex == iIndex);
			}

			bool operator!=( const iterator &oth ) const
			{
				return (oth.pHeap != pHeap) || (oth.iIndex != iIndex);
			}

			item &operator*()
			{
				pHeap->_hardCopy();

				return pHeap->core->aItem[iIndex];
			}

			item *operator->()
			{
				pHeap->_hardCopy();

				return &(pHeap->core->aItem[iIndex]);
			}

			iterator &operator++()
			{
				checkValid();
				iIndex++;
				if( iIndex >= pHeap->iFill )
					iIndex = -1;

				return *this;
			}

			iterator &operator--()
			{
				checkValid();
				iIndex--;

				return *this;
			}
			
			iterator &operator++( int )
			{
				checkValid();
				iIndex++;
				if( iIndex >= pHeap->core->iFill )
					iIndex = -1;

				return *this;
			}

			iterator &operator--( int )
			{
				checkValid();
				iIndex--;

				return *this;
			}

			iterator operator+( int iDelta )
			{
				checkValid();
				iterator ret( *this );
				ret.iIndex += iDelta;
				if( ret.iIndex >= pHeap->core->iFill )
					ret.iIndex = -1;
				return ret;
			}

			iterator operator-( int iDelta )
			{
				checkValid();
				iterator ret( *this );
				ret.iIndex -= iDelta;
				if( ret.iIndex < 0 )
					ret.iIndex = -1;
				return ret;
			}

			operator bool() const
			{
				return iIndex != -1;
			}

			bool isValid() const
			{
				return iIndex != -1;
			}

			iterator &operator=( const iterator &oth )
			{
				pHeap = oth.pHeap;
				iIndex = oth.iIndex;
			}
		};
		
		class const_iterator
		{
			friend class Heap<item, cmpfunc, itemalloc>;
		private:
			Heap<item, cmpfunc, itemalloc> *pHeap;
			int iIndex;

			const_iterator( Heap<item, cmpfunc, itemalloc> *pHeap,
					int iIndex ) :
				pHeap( pHeap ), iIndex( iIndex )
			{
			}

			void checkValid()
			{
				if( pHeap == NULL )
					throw Bu::ExceptionBase("Iterator not initialized.");
				if( iIndex < 0 || iIndex >= pHeap->core->iFill )
					throw Bu::ExceptionBase("Iterator out of bounds.");
			}

		public:
			const_iterator() :
				pHeap( NULL ),
				iIndex( -1 )
			{
			}
			
			const_iterator( const const_iterator &i ) :
				pHeap( i.pHeap ),
				iIndex( i.iIndex )
			{
			}
			
			const_iterator( const iterator &i ) :
				pHeap( i.pHeap ),
				iIndex( i.iIndex )
			{
			}

			bool operator==( const const_iterator &oth ) const
			{
				return (oth.pHeap == pHeap) && (oth.iIndex == iIndex);
			}

			bool operator!=( const const_iterator &oth ) const
			{
				return (oth.pHeap != pHeap) || (oth.iIndex != iIndex);
			}

			const item &operator*()
			{
				pHeap->_hardCopy();

				return pHeap->core->aItem[iIndex];
			}

			const item *operator->()
			{
				pHeap->_hardCopy();

				return &(pHeap->core->aItem[iIndex]);
			}

			const_iterator &operator++()
			{
				checkValid();
				iIndex++;
				if( iIndex >= pHeap->core->iFill )
					iIndex = -1;

				return *this;
			}

			const_iterator &operator--()
			{
				checkValid();
				iIndex--;

				return *this;
			}
			
			const_iterator &operator++( int )
			{
				checkValid();
				iIndex++;
				if( iIndex >= pHeap->core->iFill )
					iIndex = -1;

				return *this;
			}

			const_iterator &operator--( int )
			{
				checkValid();
				iIndex--;

				return *this;
			}

			const_iterator operator+( int iDelta )
			{
				checkValid();
				const_iterator ret( *this );
				ret.iIndex += iDelta;
				if( ret.iIndex >= pHeap->iFill )
					ret.iIndex = -1;
				return ret;
			}

			const_iterator operator-( int iDelta )
			{
				checkValid();
				const_iterator ret( *this );
				ret.iIndex -= iDelta;
				if( ret.iIndex < 0 )
					ret.iIndex = -1;
				return ret;
			}

			operator bool() const
			{
				return iIndex != -1;
			}

			bool isValid() const
			{
				return iIndex != -1;
			}

			const_iterator &operator=( const const_iterator &oth )
			{
				pHeap = oth.pHeap;
				iIndex = oth.iIndex;
			}

			const_iterator &operator=( const iterator &oth )
			{
				pHeap = oth.pHeap;
				iIndex = oth.iIndex;
			}
		};

		iterator begin()
		{
			if( core->iFill == 0 )
				return end();
			return iterator( this, 0 );
		}

		const_iterator begin() const
		{
			if( core->iFill == 0 )
				return end();
			return const_iterator( this, 0 );
		}

		iterator end()
		{
			return iterator( this, -1 );
		}

		const_iterator end() const
		{
			return const_iterator( this, -1 );
		}


	protected:
		virtual Core *_copyCore( Core *src )
		{
			Core *pRet = _allocateCore();

			pRet->iSize = src->iSize;
			pRet->iFill = src->iFill;
			pRet->cmp = src->cmp;
			pRet->aItem = pRet->ia.allocate( pRet->iSize );
			for( int j = 0; j < pRet->iFill; j++ )
			{
				pRet->ia.construct( &pRet->aItem[j], src->aItem[j] );
			}

			return pRet;
		}
	};
};

#endif
