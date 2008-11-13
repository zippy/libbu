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
	subExceptionDecl( ArrayException )
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
			*this = src;
		}
		
		Array( long iSetCap ) :
			pData( NULL ),
			iSize( 0 ),
			iCapacity( 0 )
		{
			setCapacity( iSetCap );
		}

		~Array()
		{
			clear();
		}

		MyType &operator=( const MyType &src )
		{
			clear();
			setCapacity( src.iCapacity );

			long iTop = src.getSize();
			for( long i = 0; i < iTop; ++i )
			{
				append( src[i] );
			}
			return *this;
		}

		/**
		 * Clear the array.
		 */
		void clear()
		{
			if( pData )
			{
				for( int j = 0; j < iSize; j++ )
				{
					va.destroy( &pData[j] );
				}
				va.deallocate( pData, iCapacity );
				pData = NULL;
			}
			iSize = 0;
			iCapacity = 0;
		}

		void append( const value &rVal )
		{
			if( iSize == iCapacity )
			{
				setCapacity( iCapacity + inc );
			}

			va.construct( &pData[iSize++], rVal );
		}

		//operator
		value &operator[]( long iIndex )
		{
			if( iIndex < 0 || iIndex >= iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, iSize );

			return pData[iIndex];
		}
		
		const value &operator[]( long iIndex ) const
		{
			if( iIndex < 0 || iIndex >= iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, iSize );

			return pData[iIndex];
		}

		/**
		 * Get the current size of the array.
		 *@returns The current size of the array.
		 */
		long getSize() const
		{
			return iSize;
		}

		/**
		 * Get the capacity of the array.  This number will grow as data is
		 * added, and is mainly for the curious, it doesn't really determine
		 * much for the end user.
		 *@returns The current capacity of the array.
		 */
		long getCapacity() const
		{
			return iCapacity;
		}

		/**
		 * Change the capacity of the array, very useful if you know you'll be
		 * adding a large amount of already counted items to the array, makes
		 * the appending much faster afterwords.
		 *@param iNewLen The new capacity of the array.
		 *@todo Set this up so it can reduce the size of the array as well as
		 * make it bigger.
		 */
		void setCapacity( long iNewLen )
		{
			if( iNewLen < iCapacity ) return;
			value *pNewData = va.allocate( iNewLen );
			if( pData )
			{
				for( int j = 0; j < iSize; j++ )
				{
					va.construct( &pNewData[j], pData[j] );
					va.destroy( &pData[j] );
				}
				va.deallocate( pData, iCapacity );
			}
			pData = pNewData;
			iCapacity = iNewLen;
		}

		typedef struct iterator
		{
			friend class Array<value, inc, valuealloc>;
		private:
			iterator( MyType &src, long iPos=0 ) :
				src( src ),
				iPos( iPos )
			{
				if( this->iPos >= src.getSize() )
					this->iPos = -1;
			}

			MyType &src;
			long iPos;

		public:
			iterator operator++( int )
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot increment iterator past end of array.");
					iPos++;
				if( iPos >= src.getSize() )
					iPos = -1;
				return *this;
			}
			
			iterator operator++()
			{
				if( iPos >= 0 )
					iPos++;
				if( iPos >= src.getSize() )
					iPos = -1;
				return *this;
			}
			
			iterator operator--( int )
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot increment iterator past end of array.");
					iPos--;
				if( iPos < 0 )
					iPos = -1;
				return *this;
			}
			
			iterator operator--()
			{
				if( iPos < src.getSize() )
					iPos--;
				if( iPos <= 0 )
					iPos = -1;
				return *this;
			}

			bool operator==( const iterator &oth )
			{
				return iPos == oth.iPos;
			}

			bool operator!=( const iterator &oth )
			{
				return iPos != oth.iPos;
			}

			iterator operator=( const iterator &oth )
			{
				if( &src != &oth.src )
					throw ArrayException(
						"Cannot mix iterators from different array objects.");
				iPos = oth.iPos;
			}

			value &operator*()
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot dereference finished iterator.");
				return src[iPos];
			}
		} iterator;
		
		typedef struct const_iterator
		{
			friend class Array<value, inc, valuealloc>;
		private:
			const_iterator( const MyType &src, long iPos=0 ) :
				src( src ),
				iPos( iPos )
			{
				if( this->iPos >= src.getSize() )
					this->iPos = -1;
			}

			const MyType &src;
			long iPos;

		public:
			const_iterator operator++( int )
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot increment iterator past end of array.");
					iPos++;
				if( iPos >= src.getSize() )
					iPos = -1;
				return *this;
			}
			
			const_iterator operator++()
			{
				if( iPos >= 0 )
					iPos++;
				if( iPos >= src.getSize() )
					iPos = -1;
				return *this;
			}
			
			const_iterator operator--( int )
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot increment iterator past end of array.");
					iPos--;
				if( iPos < 0 )
					iPos = -1;
				return *this;
			}
			
			const_iterator operator--()
			{
				if( iPos < src.getSize() )
					iPos--;
				if( iPos <= 0 )
					iPos = -1;
				return *this;
			}

			bool operator==( const const_iterator &oth )
			{
				return iPos == oth.iPos;
			}

			bool operator!=( const const_iterator &oth )
			{
				return iPos != oth.iPos;
			}

			const_iterator operator=( const const_iterator &oth )
			{
				if( &src != &oth.src )
					throw ArrayException(
						"Cannot mix iterators from different array objects.");
				iPos = oth.iPos;
			}

			const value &operator*() const
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot dereference finished iterator.");
				return src[iPos];
			}
		} const_iterator;

		iterator begin()
		{
			return iterator( *this );
		}

		const_iterator begin() const
		{
			return const_iterator( *this );
		}

		iterator end()
		{
			return iterator( *this, -1 );
		}

		const_iterator end() const
		{
			return const_iterator( *this, -1 );
		}

		/**
		 * If order is important, use this.  It will delete the suggested item
		 * and move the rest of the data up a spot.  This is a time O(n)
		 * operation.  If the order isn't important, check swapErase
		 */
		void erase( iterator i )
		{
			for( int j = i.iPos; j < iSize; j++ )
			{
				va.destroy( &pData[j] );
				if( j == iSize-1 )
				{
					iSize--;
					return;
				}
				va.construct( &pData[j], pData[j+1] );
			}
		}

		/**
		 * In order to make swapErase faster, what it does is swap the given
		 * item in the array with the last item, then make the array shorter
		 * by one.  It changes the order of the elements in the array, so it
		 * should be used carefully, but it is time O(1) instead of O(n) like
		 * erase.
		 */
		void swapErase( iterator i )
		{
			if( i.iPos == iSize-1 )
			{
				erase( i );
				return;
			}
			va.destroy( &pData[i.iPos] );
			va.construct( &pData[i.iPos], pData[iSize-1] );
			va.destroy( &pData[iSize-1] );
			iSize--;
		}

	private:
		valuealloc va;
		value *pData;
		long iSize;
		long iCapacity;
	};
}

#endif
