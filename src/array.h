/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ARRAY_H
#define BU_ARRAY_H

#include <memory>
#include "bu/exceptionbase.h"
#include "bu/archivebase.h"
#include "bu/sharedcore.h"

namespace Bu
{
	subExceptionDecl( ArrayException )

	template<typename value, int inc, typename valuealloc>
	class Array;

	/** @cond DEVEL */
	template<typename value, int inc, typename valuealloc>
	class ArrayCore
	{
	friend class Array<value, inc, valuealloc>;
	friend class SharedCore<
		Array<value, inc, valuealloc>,
		ArrayCore<value, inc, valuealloc>
		>;
	private:		
		ArrayCore() :
			pData( NULL ),
			iSize( 0 ),
			iCapacity( 0 )
		{ }

		void setCapacity( int iNewLen )
		{
			//clear();
			//iCapacity = iCapacity;
			//pData = va.allocate( iCapacity );
			if( iNewLen <= iCapacity ) return;
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

		virtual ~ArrayCore()
		{
			clear();
		}

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

		void erase( int iPos )
		{
			for( int j = iPos; j < iSize; j++ )
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

		void swapErase( int iPos )
		{
			if( iPos == iSize-1 )
			{
				erase( iPos );
				return;
			}
			va.destroy( &pData[iPos] );
			va.construct( &pData[iPos], pData[iSize-1] );
			va.destroy( &pData[iSize-1] );
			iSize--;
		}

		valuealloc va;
		value *pData;
		long iSize;
		long iCapacity;
	};
	/** @endcond */

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
	class Array : public SharedCore<
				  Array<value, inc, valuealloc>,
				  ArrayCore<value, inc, valuealloc>
				  >
	{
	private:
		typedef class Array<value, inc, valuealloc> MyType;
		typedef class ArrayCore<value, inc, valuealloc> Core;

	protected:
		using SharedCore<MyType, Core>::core;
		using SharedCore<MyType, Core>::_hardCopy;
		using SharedCore<MyType, Core>::_resetCore;
		using SharedCore<MyType, Core>::_allocateCore;

	public:
		struct const_iterator;
		struct iterator;

		Array()
		{
		}

		Array( const MyType &src ) :
			SharedCore<MyType, Core >( src )
		{
		}
		
		Array( long iSetCap )
		{
			setCapacity( iSetCap );
		}

		~Array()
		{
		}

		bool operator==( const MyType &src ) const
		{
			if( core == src.core )
				return true;
			if( core->iSize != src.core->iSize )
				return false;

			for( int j = 0; j < core->iSize; j++ )
			{
				if( core->pData[j] != src.core->pData[j] )
					return false;
			}
			return true;
		}

		bool operator!=( const MyType &src ) const
		{
			return !(*this == src);
		}

		/**
		 * Clear the array.
		 */
		void clear()
		{
			_resetCore();
		}

		MyType &append( const value &rVal )
		{
			_hardCopy();
			if( core->iSize == core->iCapacity )
			{
				core->setCapacity( core->iCapacity + inc );
			}

			core->va.construct( &core->pData[core->iSize++], rVal );

			return *this;
		}
		
		MyType &append( const MyType &rVal )
		{
			_hardCopy();

			if( core->iSize + rVal.core->iSize > core->iCapacity )
			{
				core->setCapacity( core->iSize + rVal.core->iSize + inc );
			}
			
			for( int j = 0; j < rVal.core->iSize; j++ )
			{
				core->va.construct(
					&core->pData[core->iSize++],
					rVal.core->pData[j]
					);
			}

			return *this;
		}

		//operator
		value &operator[]( long iIndex )
		{
			_hardCopy();
			if( iIndex < 0 || iIndex >= core->iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, core->iSize );

			return core->pData[iIndex];
		}
		
		const value &operator[]( long iIndex ) const
		{
			if( iIndex < 0 || iIndex >= core->iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, core->iSize );

			return core->pData[iIndex];
		}

		value &get( long iIndex )
		{
			_hardCopy();
			if( iIndex < 0 || iIndex >= core->iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, core->iSize );

			return core->pData[iIndex];
		}

		const value &get( long iIndex ) const
		{
			if( iIndex < 0 || iIndex >= core->iSize )
				throw ArrayException(
					"Index %d out of range 0:%d", iIndex, core->iSize );

			return core->pData[iIndex];
		}

		value &first()
		{
			_hardCopy();
			return core->pData[0];
		}

		const value &first() const
		{
			return core->pData[0];
		}

		value &last()
		{
			_hardCopy();
			return core->pData[core->iSize-1];
		}

		const value &last() const
		{
			return core->pData[core->iSize-1];
		}

		/**
		 * Get the current size of the array.
		 *@returns The current size of the array.
		 */
		long getSize() const
		{
			return core->iSize;
		}

		/**
		 * Get the capacity of the array.  This number will grow as data is
		 * added, and is mainly for the curious, it doesn't really determine
		 * much for the end user.
		 *@returns The current capacity of the array.
		 */
		long getCapacity() const
		{
			return core->iCapacity;
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
			_hardCopy();
			core->setCapacity( iNewLen );
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
			
			iterator operator+( int iAmnt )
			{
				if( iPos < 0 )
					throw ArrayException(
						"Cannot increment iterator past end of array.");
					iPos += iAmnt;
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
			
			iterator operator-( int iAmnt )
			{
				if( iPos < src.getSize() )
					iPos -= iAmnt;
				if( iPos <= 0 )
					iPos = -1;
				return *this;
			}

			bool operator==( const iterator &oth ) const
			{
				return iPos == oth.iPos;
			}

			bool operator!=( const iterator &oth ) const
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

			long getIndex() const
			{
				return iPos;
			}

			operator bool() const
			{
				return iPos >= 0;
			}

			bool isValid() const
			{
				return iPos >= 0;
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
			const_iterator( iterator &rSrc ) :
				src( rSrc.src ),
				iPos( rSrc.iPos )
			{
			}
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

			bool operator==( const const_iterator &oth ) const
			{
				return iPos == oth.iPos;
			}

			bool operator!=( const const_iterator &oth ) const
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

			long getIndex() const
			{
				return iPos;
			}
			
			operator bool() const
			{
				return iPos >= 0;
			}

			bool isValid() const
			{
				return iPos >= 0;
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

		MyType &insert( iterator i, const value &rVal )
		{
			if( i.iPos == -1 )
			{
				append( rVal );
				return *this;
			}

			_hardCopy();
			if( core->iSize == core->iCapacity )
			{
				core->setCapacity( core->iCapacity + inc );
			}
			core->iSize++;

			core->va.construct(
				&core->pData[core->iSize-1],
				core->pData[core->iSize-2]
				);
			for( int iPos = core->iSize-2; iPos > i.iPos; iPos-- )
			{
				core->va.destroy( &core->pData[iPos] );
				core->va.construct( &core->pData[iPos], core->pData[iPos-1] );
			}
			core->va.destroy( &core->pData[i.iPos] );
			core->va.construct( &core->pData[i.iPos], rVal );

			return *this;
		}

		/**
		 * If order is important, use this.  It will delete the suggested item
		 * and move the rest of the data up a spot.  This is a time O(n)
		 * operation.  If the order isn't important, check swapErase
		 */
		void erase( iterator i )
		{
			_hardCopy();
			core->erase( i.iPos );
		}

		void erase( const value &v )
		{
			_hardCopy();
			for( int j = 0; j < core->iSize; j++ )
			{
				if( core->pData[j] == v )
				{
					core->erase( j );
					return;
				}
			}
		}

		void eraseLast()
		{
			_hardCopy();
			core->erase( core->iSize-1 );
		}

		void eraseFirst()
		{
			_hardCopy();
			core->erase( 0 );
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
			_hardCopy();
			core->swapErase( i.iPos );
		}

	protected:
		virtual Core *_copyCore( Core *src )
		{
			Core *pRet = _allocateCore();
			pRet->setCapacity( src->iCapacity );
			pRet->iSize = src->iSize;
			for( int j = 0; j < src->iSize; j++ )
			{	
				pRet->va.construct( &pRet->pData[j], src->pData[j] );
			}
			return pRet;
		}

	private:
	};

	class Formatter;
	Formatter &operator<<( Formatter &rOut, char *sStr );
	Formatter &operator<<( Formatter &rOut, signed char c );
	template<typename value>
	Formatter &operator<<( Formatter &f, const Bu::Array<value> &a )
	{
		f << '[';
		for( typename Bu::Array<value>::const_iterator i = a.begin(); i; i++ )
		{
			if( i != a.begin() )
				f << ", ";
			f << *i;
		}
		f << ']';

		return f;
	}

	template<typename value, int inc, typename valuealloc>
	ArchiveBase &operator<<( ArchiveBase &ar,
			const Array<value, inc, valuealloc> &h )
	{
		ar << h.getSize();
		for( typename Array<value, inc, valuealloc>::const_iterator i =
			 h.begin(); i != h.end(); i++ )
		{
			ar << (*i);
		}

		return ar;
	}

	template<typename value, int inc, typename valuealloc>
	ArchiveBase &operator>>(ArchiveBase &ar, Array<value, inc, valuealloc> &h )
	{
		h.clear();
		long nSize;
		ar >> nSize;

		h.setCapacity( nSize );
		for( long j = 0; j < nSize; j++ )
		{
			value v;
			ar >> v;
			h.append( v );
		}
		return ar;
	}

}

#endif
