/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_RING_BUFFER_H
#define BU_RING_BUFFER_H

#include <memory>
#include "bu/exceptionbase.h"
#include "bu/queue.h"
#include "bu/sharedcore.h"

namespace Bu
{
	template<typename value, typename valuealloc> class RingBuffer;

	/** @cond DEVEL */
	template<typename value, typename valuealloc>
	class RingBufferCore
	{
	friend class RingBuffer<value, valuealloc>;
	friend class SharedCore<RingBuffer<value, valuealloc>,
		   RingBufferCore<value, valuealloc> >;
	private:
		RingBufferCore() :
			iCapacity( 0 ),
			iStart( -1 ),
			iEnd( -2 ),
			aData( NULL )
		{
		}

		virtual ~RingBufferCore()
		{
			clear();
		}

		void init( int iNewCapacity )
		{
			if( iCapacity > 0 )
				return;

			iCapacity = iNewCapacity;
			iStart = -1;
			iEnd = -2;
			aData = va.allocate( iCapacity );
		}

		void clear()
		{
			for( int j = iStart; j < iEnd; j=(j+1%iCapacity) )
			{
				va.destroy( &aData[j] );
			}
			va.deallocate( aData, iCapacity );
			aData = NULL;
			iCapacity = 0;
		}
		
		void enqueue( const value &v )
		{
			if( iStart == -1 )
			{
				iStart = 0;
				iEnd = 1;
				va.construct( &aData[0], v );
			}
			else if( iStart == iEnd )
			{
				throw ExceptionBase("Hey, it's full!");
			}
			else
			{
				va.construct( &aData[iEnd], v );
				iEnd = (iEnd+1)%iCapacity;
			}
		}

		value dequeue()
		{
			if( iStart == -1 )
			{
				throw ExceptionBase("No data");
			}
			else
			{
				value &v = aData[iStart];
				va.destroy( &aData[iStart] );
				iStart = (iStart+1)%iCapacity;
				if( iStart == iEnd )
				{
					iStart = -1;
					iEnd = -2;
				}
				return v;
			}
		}

		value &get( int iIndex )
		{
			return aData[(iIndex+iStart)%iCapacity];
		}

		int getSize()
		{
			if( iStart < 0 )
				return 0;
			if( iEnd == iStart )
				return iCapacity;
			if( iEnd < iStart )
				return iEnd-iStart;
			return iCapacity-(iEnd-iStart);
		}

		int iCapacity;
		int iStart, iEnd;
		value *aData;
		valuealloc va;
	};
	/** @endcond */

	/**
	 *@ingroup Containers
	 */
	template<typename value, typename valuealloc=std::allocator<value> >
	class RingBuffer : public Queue<value>, public SharedCore<
					   RingBuffer<value, valuealloc>,
					   RingBufferCore<value, valuealloc>
					   >
	{
	private:
		typedef RingBuffer<value, valuealloc> MyType;
		typedef RingBufferCore<value, valuealloc> Core;

	protected:
		using SharedCore<MyType, Core>::core;
		using SharedCore<MyType, Core>::_hardCopy;
		using SharedCore<MyType, Core>::_allocateCore;

	public:
		RingBuffer( int iCapacity )
		{
			core->init( iCapacity );
		}

		RingBuffer( const RingBuffer &rSrc ) :
			SharedCore<MyType, Core>( rSrc )
		{
		}

		virtual ~RingBuffer()
		{
		}

		int getCapacity() const
		{
			return core->iCapacity;
		}

		bool isFilled() const
		{
			return (core->iStart == core->iEnd);
		}

		bool isEmpty() const
		{
			return (core->iStart == -1);
		}

		virtual void enqueue( const value &v )
		{
			_hardCopy();

			core->enqueue( v );
		}

		virtual value dequeue()
		{
			_hardCopy();

			return core->dequeue();
		}

		virtual int getSize() const
		{
			return core->getSize();
		}

		virtual value &peek()
		{
			_hardCopy();

			return core->get( 0 );
		}

		virtual const value &peek() const
		{
			return core->get( 0 );
		}

		value &operator[]( int iIndex )
		{
			_hardCopy();

			return core->get( iIndex );
		}

	protected:
		virtual Core *_copyCore( Core *src )
		{
			Core *pRet = _allocateCore();

			pRet->init( src->iCapacity );
			int iSize = src->getSize();
			for( int j = 0; j < iSize; j++ )
			{
				pRet->enqueue( src->get( j ) );
			}

			return pRet;
		}
	};
}

#endif
