/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_HEAP_H
#define BU_HEAP_H

#include <stddef.h>
#include <string.h>
#include <memory>
#include <iostream>
#include "bu/exceptionbase.h"
#include "bu/util.h"

namespace Bu
{
	subExceptionDecl( HeapException );

	template<typename item>
	struct __basicLTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return a <= b;
		}
	};
	
	template<typename item>
	struct __basicGTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return a >= b;
		}
	};

	template<typename item>
	struct __basicPtrLTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return *a <= *b;
		}
	};
	
	template<typename item>
	struct __basicPtrGTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return *a >= *b;
		}
	};

	template<typename item, typename cmpfunc=__basicLTCmp<item>, typename itemalloc=std::allocator<item> >
	class Heap
	{
	public:
		Heap() :
			iSize( 7 ),
			iFill( 0 ),
			aItem( ia.allocate( iSize ) )
		{
		}

		virtual ~Heap()
		{
			for( int j = 0; j < iFill; j++ )
				ia.destroy( &aItem[j] );
			ia.deallocate( aItem, iSize );
			aItem = NULL;
		}

		void push( item i )
		{
			if( iFill+1 >= iSize )
				upSize();

			for( int j = 0; j < iFill; )
			{
				if( cmp( i, aItem[j] ) )
				{
					swap( i, aItem[j] );
				}

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
			for( int j = iFill; j >= 0; )
			{
				int k = (j-1)/2;
				if( cmp( aItem[k], aItem[j] ) )
					break;

				swap( aItem[k], aItem[j] );
				j = k;
			}
			iFill++;
		}

		item &peek()
		{
			if( iFill == 0 )
				throw HeapException("Heap empty.");
			return aItem[0];
		}

		void pop()
		{
			if( iFill == 0 )
				throw HeapException("Heap empty.");
			int j;
			for( j = 0; j < iFill; )
			{
				if( cmp( aItem[j*2+2], aItem[j*2+1] ) && j*2+2 < iFill )
				{
					aItem[j] = aItem[j*2+2];
					j = j*2+2;
				}
				else if( j*2+1 < iFill )
				{
					aItem[j] = aItem[j*2+1];
					j = j*2+1;
				}
				else
					break;
			}
			aItem[j] = aItem[iFill-1];
			ia.destroy( &aItem[iFill-1] );
			iFill--;
		}

		void print()
		{
			printf("graph G {\n");
			for( int j = 0; j < iFill; j++ )
			{
				if( j*2+1 < iFill )
					printf("    %d -- %d;\n",
							j, j*2+1
						  );
				if( j*2+2 < iFill )
					printf("    %d -- %d;\n",
							j, j*2+2
						  );
			}
			for( int j = 0; j < iFill; j++ )
			{
				printf("    %d [label=\"%d\"];\n",
						j, aItem[j]
					  );
			}
			printf("}\n");
		}

	private:
		void upSize()
		{
			item *aNewItems = ia.allocate( iSize*2+1 );
			memcpy( aNewItems, aItem, sizeof(item)*iFill );
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
