#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <memory>
#include "bu/exceptionbase.h"

namespace Bu
{
	template<typename value, typename valuealloc=std::allocator<value> >
	class RingBuffer
	{
	public:
		RingBuffer( int nCapacity ) :
			nCapacity( nCapacity ),
			nStart( -1 ),
			nEnd( -2 )
		{
			aData = va.allocate( nCapacity );
			for( int j = 0; j < nCapacity; j++ )
			{
				va.construct( &aData[j], value() );
			}
		}

		virtual ~RingBuffer()
		{
			for( int j = 0; j < nCapacity; j++ )
			{
				va.destroy( &aData[j] );
			}
			va.deallocate( aData, nCapacity );
		}

		int getCapacity()
		{
			return nCapacity;
		}

		bool isFilled()
		{
			return (nStart == nEnd);
		}

		bool isEmpty()
		{
			return (nStart == -1);
		}

		void enqueue( const value &v )
		{
			if( nStart == -1 )
			{
				nStart = 0;
				nEnd = 1;
				aData[0] = v;
			}
			else if( nStart == nEnd )
			{
				throw ExceptionBase("Hey, it's full!");
			}
			else
			{
				aData[nEnd] = v;
				nEnd = (nEnd+1)%nCapacity;
			}
		}

		value dequeue()
		{
			if( nStart == -1 )
			{
				throw ExceptionBase("No data");
			}
			else
			{
				value &v = aData[nStart];
				nStart = (nStart+1)%nCapacity;
				if( nStart == nEnd )
				{
					nStart = -1;
					nEnd = -2;
				}
				return v;
			}
		}

		value &operator[]( int nIndex )
		{
			return aData[(nIndex+nStart)%nCapacity];
		}

	private:
		int nCapacity;
		value *aData;
		valuealloc va;
		int nStart, nEnd;
	};
}

#endif
