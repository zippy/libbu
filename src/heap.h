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

namespace Bu
{
	subExceptionDecl( HeapException );

	template<typename item, typename cmpfunc=__basicLTCmp<item>, typename itemalloc=std::allocator<item> >
	class Heap : public Queue<item>
	{
	public:
		Heap() :
			iSize( 7 ),
			iFill( 0 ),
			aItem( ia.allocate( iSize ) )
		{
		}
		
		Heap( cmpfunc cmpin ) :
			iSize( 7 ),
			iFill( 0 ),
			aItem( ia.allocate( iSize ) ),
			cmp( cmpin )
		{
		}

		Heap( int iInitialCapacity ) :
			iSize( 0 ),
			iFill( 0 ),
			aItem( NULL )
		{
			for( iSize = 1; iSize < iInitialCapacity; iSize=iSize*2+1 ) { }
			aItem = ia.allocate( iSize );
		}
		
		Heap( cmpfunc cmpin, int iInitialCapacity ) :
			iSize( 0 ),
			iFill( 0 ),
			aItem( NULL ),
			cmp( cmpin )
		{
			for( iSize = 1; iSize < iInitialCapacity; iSize=iSize*2+1 ) { }
			aItem = ia.allocate( iSize );
		}

		virtual ~Heap()
		{
			for( int j = 0; j < iFill; j++ )
				ia.destroy( &aItem[j] );
			ia.deallocate( aItem, iSize );
			aItem = NULL;
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

		virtual item &peek()
		{
			if( iFill == 0 )
				throw HeapException("Heap empty.");
			return aItem[0];
		}

		virtual const item &peek() const
		{
			if( iFill == 0 )
				throw HeapException("Heap empty.");
			return aItem[0];
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

		virtual bool isEmpty() const
		{
			return (iFill==0);
		}

		virtual int getSize() const
		{
			return iFill;
		}

		/*
		void print( class Formatter &f )
		{
			f << "graph G {" << "\n";
			for( int j = 0; j < iFill; j++ )
			{
				if( j*2+1 < iFill )
					f << "    " << j << " -- " << j*2+1 << ";" << "\n";
				if( j*2+2 < iFill )
					f << "    " << j << " -- " << j*2+2 << ";" << "\n";
			}
			for( int j = 0; j < iFill; j++ )
			{
				f << "    " << j << " [label=\"" << aItem[j] << "\"];" << "\n";
			}
			f << "}" << "\n";
		} */

	private:
		void upSize()
		{
			item *aNewItems = ia.allocate( iSize*2+1 );
//			memcpy( aNewItems, aItem, sizeof(item)*iFill );
			for( int j = 0; j < iFill; j++ )
			{
				ia.construct( &aNewItems[j], aItem[j] );
				ia.destroy( &aItem[j] );
			}
			ia.deallocate( aItem, iSize );
			aItem = aNewItems;
			iSize = iSize*2+1;
		}

	private:
		int iSize;
		int iFill;
		item *aItem;
		cmpfunc cmp;
		itemalloc ia;
	};
};

#endif
