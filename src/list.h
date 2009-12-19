/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_LIST_H
#define BU_LIST_H

#include <memory>
#include "bu/exceptionbase.h"
#include "bu/sharedcore.h"
#include "bu/archivebase.h"
#include "bu/heap.h"

namespace Bu
{
	template<typename value>
	struct ListLink
	{
		value *pValue;
		ListLink *pNext;
		ListLink *pPrev;
	};

	template<typename value, typename valuealloc,
		typename linkalloc>
	struct ListCore
	{
		typedef struct ListLink<value> Link;
		ListCore() :
			pFirst( NULL ),
			pLast( NULL ),
			nSize( 0 )
		{ }

		virtual ~ListCore()
		{
			clear();
		}

		Link *pFirst;
		Link *pLast;
		long nSize;
		linkalloc la;
		valuealloc va;
		
		/**
		 * Append a value to the list.
		 *@param v (const value_type &) The value to append.
		 */
		void append( const value &v )
		{
			Link *pNew = la.allocate( 1 );
			pNew->pValue = va.allocate( 1 );
			va.construct( pNew->pValue, v );
			nSize++;
			if( pFirst == NULL )
			{
				// Empty list
				pFirst = pLast = pNew;
				pNew->pNext = pNew->pPrev = NULL;
			}
			else
			{
				pNew->pNext = NULL;
				pNew->pPrev = pLast;
				pLast->pNext = pNew;
				pLast = pNew;
			}
		}
		
		/**
		 * Prepend a value to the list.
		 *@param v (const value_type &) The value to prepend.
		 */
		void prepend( const value &v )
		{
			Link *pNew = la.allocate( 1 );
			pNew->pValue = va.allocate( 1 );
			va.construct( pNew->pValue, v );
			nSize++;
			if( pFirst == NULL )
			{
				// Empty list
				pFirst = pLast = pNew;
				pNew->pNext = pNew->pPrev = NULL;
			}
			else
			{
				pNew->pNext = pFirst;
				pNew->pPrev = NULL;
				pFirst->pPrev = pNew;
				pFirst = pNew;
			}
		}
		
		void clear()
		{
			Link *pCur = pFirst;
			for(;;)
			{
				if( pCur == NULL ) break;
				va.destroy( pCur->pValue );
				va.deallocate( pCur->pValue, 1 );
				Link *pTmp = pCur->pNext;
				la.destroy( pCur );
				la.deallocate( pCur, 1 );
				pCur = pTmp;
			}
			pFirst = pLast = NULL;
			nSize = 0;
		}
		
		void insert( Link *pLink, const value &v )
		{
			Link *pAfter = pLink;
			if( pAfter == NULL )
			{
				append( v );
				return;
			}
			Link *pPrev = pAfter->pPrev;
			if( pPrev == NULL )
			{
				prepend( v );
				return;
			}

			Link *pNew = la.allocate( 1 );
			pNew->pValue = va.allocate( 1 );
			va.construct( pNew->pValue, v );
			nSize++;
		
			pNew->pNext = pAfter;
			pNew->pPrev = pPrev;
			pAfter->pPrev = pNew;
			pPrev->pNext = pNew;
		}

		/**
		 * Erase an item from the list.
		 *@param i (iterator) The item to erase.
		 */
		void erase( Link *pLink )
		{
			Link *pCur = pLink;
			if( pCur == NULL ) return;
			Link *pPrev = pCur->pPrev;
			if( pPrev == NULL )
			{
				va.destroy( pCur->pValue );
				va.deallocate( pCur->pValue, 1 );
				pFirst = pCur->pNext;
				la.destroy( pCur );
				la.deallocate( pCur, 1 );
				if( pFirst == NULL )
					pLast = NULL;
				else
					pFirst->pPrev = NULL;
				nSize--;
			}
			else
			{
				va.destroy( pCur->pValue );
				va.deallocate( pCur->pValue, 1 );
				Link *pTmp = pCur->pNext;
				la.destroy( pCur );
				la.deallocate( pCur, 1 );
				pPrev->pNext = pTmp;
				if( pTmp != NULL )
					pTmp->pPrev = pPrev;
				nSize--;
			}
		}
	};

	/**
	 * Linked list template container.  This class is similar to the stl list
	 * class except for a few minor changes.  First, when const, all
	 * members are only accessable const.  Second, erasing a location does not
	 * invalidate the iterator used, it simply points to the next valid
	 * location, or end() if there are no more.  Other iterators pointing to
	 * the deleted record will, of course, no longer be valid.
	 *
	 *@param value (typename) The type of data to store in your list
	 *@param valuealloc (typename) Memory Allocator for your value type
	 *@param linkalloc (typename) Memory Allocator for the list links.
	 *@ingroup Containers
	 */
	template<typename value, typename valuealloc=std::allocator<value>,
		typename linkalloc=std::allocator<struct ListLink<value> > >
	class List : public SharedCore< struct ListCore<value, valuealloc,
		linkalloc> >
	{
	private:
		typedef struct ListLink<value> Link;
		typedef class List<value, valuealloc, linkalloc> MyType;
		typedef struct ListCore<value, valuealloc, linkalloc> Core;

	protected:
		using SharedCore< Core >::core;
		using SharedCore< Core >::_hardCopy;
		using SharedCore< Core >::_allocateCore;

	public:
		struct const_iterator;
		struct iterator;

		List()
		{
		}

		List( const MyType &src ) :
			SharedCore< Core >( src )
		{
		}
		
		List( const value &v )
		{
			append( v );
		}

		~List()
		{
		}

		MyType &operator+=( const value &v )
		{
			_hardCopy();
			append( v );
			return *this;
		}

		MyType &operator+=( const MyType &src )
		{
			_hardCopy();
			append( src );
			return *this;
		}
		
		MyType operator+( const MyType &src )
		{
			MyType lNew( *this );
			lNew += src;
			return lNew;
		}

		bool operator==( const MyType &rhs ) const
		{
			if( getSize() != rhs.getSize() )
				return false;

			for( typename MyType::const_iterator a = begin(), b = rhs.begin();
				 a; a++, b++ )
			{
				if( *a != *b )
					return false;
			}

			return true;
		}

		bool operator!=( const MyType &rhs ) const
		{
			return !(*this == rhs);
		}

		/**
		 * Clear the data from the list.
		 */
		void clear()
		{
			_hardCopy();
			core->clear();
		}

		MyType &enqueue( const value &v )
		{
			_hardCopy();
			append( v );

			return *this;
		}

		value dequeue()
		{
			// _hardCopy();  erase will call this for me
			value v = *core->pFirst->pValue;

			erase( begin() );

			return v;
		}

		MyType &push( const value &v )
		{
			_hardCopy();
			prepend( v );

			return *this;
		}

		MyType &pop()
		{
			_hardCopy();
			erase( begin() );

			return *this;
		}

		value peekPop()
		{
			value v = first();
			pop();
			return v;
		}

		value &peek()
		{
			return first();
		}

		/**
		 * Append a value to the list.
		 *@param v (const value_type &) The value to append.
		 */
		MyType &append( const value &v )
		{
			_hardCopy();
			core->append( v );

			return *this;
		}

		MyType &append( const MyType &rSrc )
		{
			_hardCopy();
			for( typename MyType::const_iterator i = rSrc.begin();
				 i != rSrc.end(); i++ )
			{
				core->append( *i );
			}

			return *this;
		}

		/**
		 * Prepend a value to the list.
		 *@param v (const value_type &) The value to prepend.
		 */
		MyType &prepend( const value &v )
		{
			_hardCopy();
			core->prepend( v );

			return *this;
		}

		/**
		 * Prepend another list to the front of this one.  This will prepend
		 * the rSrc list in reverse order...I may fix that later.
		 */
		MyType &prepend( const MyType &rSrc )
		{
			_hardCopy();
			for( typename MyType::const_iterator i = rSrc.begin();
				 i != rSrc.end(); i++ )
			{
				core->prepend( *i );
			}

			return *this;
		}

		MyType &insert( MyType::iterator &i, const value &v )
		{
			_hardCopy();

			core->insert( i.pLink, v );

			return *this;
		}

		template<typename cmptype>
		void sort( cmptype cmp )
		{
			Heap<value, cmptype, valuealloc> hSort( cmp, getSize() );
			for( typename MyType::iterator i = begin(); i; i++ )
			{
				hSort.enqueue( *i );
			}
			clear();
			while( !hSort.isEmpty() )
			{
				append( hSort.dequeue() );
			}
		}

		void sort()
		{
			sort<__basicLTCmp<value> >();
		}

		template<typename cmptype>
		void sort()
		{
			Heap<value, cmptype, valuealloc> hSort( getSize() );
			for( typename MyType::iterator i = begin(); i; i++ )
			{
				hSort.enqueue( *i );
			}
			clear();
			while( !hSort.isEmpty() )
			{
				append( hSort.dequeue() );
			}
		}

		/**
		 * Insert a new item in sort order by searching for the first item that
		 * is larger and inserting this before it, or at the end if none are
		 * larger.  If this is the only function used to insert data in the
		 * List all items will be sorted.  To use this, the value type must
		 * support the > operator.
		 */
		template<typename cmptype>
		MyType &insertSorted( cmptype cmp, const value &v )
		{
			_hardCopy();
			if( core->pFirst == NULL )
			{
				// Empty list
				core->append( v );
				return *this;
			}
			else
			{
				Link *pCur = core->pFirst;
				for(;;)
				{
					if( cmp( v, *(pCur->pValue)) )
					{
						core->insert( pCur, v );
						return *this;
					}
					pCur = pCur->pNext;
					if( pCur == NULL )
					{
						core->append( v );
						return *this;
					}
				}
			}
		}
		
		MyType &insertSorted( const value &v )
		{
			return insertSorted<__basicLTCmp<value> >( v );
		}

		template<typename cmptype>
		MyType &insertSorted( const value &v )
		{
			cmptype cmp;
			return insertSorted( cmp, v );
		}

		/**
		 * An iterator to iterate through your list.
		 */
		typedef struct iterator
		{
			friend struct const_iterator;
			friend class List<value, valuealloc, linkalloc>;
		private:
			Link *pLink;

			iterator( Link *pLink ) :
				pLink( pLink )
			{
			}

		public:
			iterator() :
				pLink( NULL )
			{
			}

			iterator( const iterator &i ) :
				pLink( i.pLink )
			{
			}

			/**
			 * Equals comparison operator.
			 *@param oth (const iterator &) The iterator to compare to.
			 *@returns (bool) Are they equal?
			 */
			bool operator==( const iterator &oth ) const
			{
				return ( pLink == oth.pLink );
			}

			/**
			 * Equals comparison operator.
			 *@param pOth (const Link *) The link to compare to.
			 *@returns (bool) Are they equal?
			 */
			bool operator==( const Link *pOth ) const
			{
				return ( pLink == pOth );
			}

			/**
			 * Not equals comparison operator.
			 *@param oth (const iterator &) The iterator to compare to.
			 *@returns (bool) Are they not equal?
			 */
			bool operator!=( const iterator &oth ) const
			{
				return ( pLink != oth.pLink );
			}

			/**
			 * Not equals comparison operator.
			 *@param pOth (const Link *) The link to compare to.
			 *@returns (bool) Are they not equal?
			 */
			bool operator!=( const Link *pOth ) const
			{
				return ( pLink != pOth );
			}

			/**
			 * Dereference operator.
			 *@returns (value_type &) The value.
			 */
			value &operator*()
			{
				return *(pLink->pValue);
			}

			/**
			 * Pointer access operator.
			 *@returns (value_type *) A pointer to the value.
			 */
			value *operator->()
			{
				return pLink->pValue;
			}

			iterator &operator++()
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past end of list.");
				pLink = pLink->pNext;
				return *this;
			}

			iterator &operator--()
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past begining of list.");
				pLink = pLink->pPrev;
				return *this;
			}
			
			iterator &operator++( int )
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past end of list.");
				pLink = pLink->pNext;
				return *this;
			}

			iterator &operator--( int )
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past begining of list.");
				pLink = pLink->pPrev;
				return *this;
			}

			iterator operator+( int iDelta )
			{
				iterator ret( *this );
				for( int j = 0; j < iDelta; j++ )
				{
					if( ret.pLink == NULL )
						throw Bu::ExceptionBase(
							"Attempt to iterate past begining of list.");
					ret.pLink = ret.pLink->pNext;
				}
				return ret;
			}

			iterator operator-( int iDelta )
			{
				iterator ret( *this );
				for( int j = 0; j < iDelta; j++ )
				{
					if( ret.pLink == NULL )
						throw Bu::ExceptionBase(
							"Attempt to iterate past begining of list.");
					ret.pLink = ret.pLink->pPrev;
				}
				return ret;
			}

			operator bool()
			{
				return pLink != NULL;
			}

			bool isValid()
			{
				return pLink != NULL;
			}

			/**
			 * Assignment operator.
			 *@param oth (const iterator &) The other iterator to set this
			 *		one to.
			 */
			iterator &operator=( const iterator &oth )
			{
				pLink = oth.pLink;
				return *this;
			}
		} iterator;
		
		/**
		 *@see iterator
		 */
		typedef struct const_iterator
		{
			friend class List<value, valuealloc, linkalloc>;
		private:
			Link *pLink;

			const_iterator( Link *pLink ) :
				pLink( pLink )
			{
			}

		public:
			const_iterator() :
				pLink( NULL )
			{
			}

			const_iterator( const iterator &i ) :
				pLink( i.pLink )
			{
			}

			bool operator==( const const_iterator &oth ) const
			{
				return ( pLink == oth.pLink );
			}

			bool operator==( const Link *pOth ) const
			{
				return ( pLink == pOth );
			}

			bool operator!=( const const_iterator &oth ) const
			{
				return ( pLink != oth.pLink );
			}

			bool operator!=( const Link *pOth ) const
			{
				return ( pLink != pOth );
			}

			const value &operator*()
			{
				return *(pLink->pValue);
			}

			const value *operator->()
			{
				return pLink->pValue;
			}

			const_iterator &operator++()
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past end of list.");
				pLink = pLink->pNext;
				return *this;
			}

			const_iterator &operator--()
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past begining of list.");
				pLink = pLink->pPrev;
				return *this;
			}
			
			const_iterator &operator++( int )
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past end of list.");
				pLink = pLink->pNext;
				return *this;
			}

			const_iterator &operator--( int )
			{
				if( pLink == NULL )
					throw Bu::ExceptionBase(
						"Attempt to iterate past begining of list.");
				pLink = pLink->pPrev;
				return *this;
			}

			const_iterator operator+( int iDelta )
			{
				const_iterator ret( *this );
				for( int j = 0; j < iDelta; j++ )
				{
					if( ret.pLink == NULL )
						throw Bu::ExceptionBase(
							"Attempt to iterate past begining of list.");
					ret.pLink = ret.pLink->pNext;
				}
				return ret;
			}

			const_iterator operator-( int iDelta )
			{
				const_iterator ret( *this );
				for( int j = 0; j < iDelta; j++ )
				{
					if( ret.pLink == NULL )
						throw Bu::ExceptionBase(
							"Attempt to iterate past begining of list.");
					ret.pLink = ret.pLink->pPrev;
				}
				return ret;
			}

			const_iterator &operator=( const iterator &oth )
			{
				pLink = oth.pLink;
				return *this;
			}

			const_iterator &operator=( const const_iterator &oth )
			{
				pLink = oth.pLink;
				return *this;
			}

			operator bool()
			{
				return pLink != NULL;
			}

			bool isValid()
			{
				return pLink != NULL;
			}
		} const_iterator;

		/**
		 * Get an iterator pointing to the first item in the list.
		 *@returns (iterator)
		 */
		iterator begin()
		{
			_hardCopy();
			return iterator( core->pFirst );
		}

		/**
		 * Get a const iterator pointing to the first item in the list.
		 *@returns (const const_iterator)
		 */
		const_iterator begin() const
		{
			return const_iterator( core->pFirst );
		}

		/**
		 * Get an iterator pointing to a place just past the last item in
		 * 		the list.
		 *@returns (const Link *)
		 */
		const iterator end()
		{
			return iterator( NULL );
		}

		/**
		 * Get an iterator pointing to a place just past the last item in
		 * 		the list.
		 *@returns (const Link *)
		 */
		const const_iterator end() const
		{
			return const_iterator( NULL );
		}

		/**
		 * Erase an item from the list.
		 *@param i (iterator) The item to erase.
		 */
		MyType &erase( iterator i )
		{
			_hardCopy();
			core->erase( i.pLink );

			return *this;
		}

		/**
		 * Erase an item from the list.
		 *@param i (iterator) The item to erase.
		 */
		MyType &erase( const_iterator i )
		{
			_hardCopy();
			core->erase( i.pLink );

			return *this;
		}

		/**
		 * Erase an item from the list if you already know the item.
		 *@param v The item to find and erase.
		 */
		MyType &erase( const value &v )
		{
			for( const_iterator i = begin(); i != end(); i++ )
			{
				if( (*i) == v )
				{
					erase( i );
					return *this;
				}
			}

			return *this;
		}

		/**
		 * Get the current size of the list.
		 *@returns (int) The current size of the list.
		 */
		long getSize() const
		{
			return core->nSize;
		}

		/**
		 * Get the first item in the list.
		 *@returns (value_type &) The first item in the list.
		 */
		value &first()
		{
			_hardCopy();
			return *core->pFirst->pValue;
		}
		
		/**
		 * Get the first item in the list.
		 *@returns (const value_type &) The first item in the list.
		 */
		const value &first() const
		{
			return *core->pFirst->pValue;
		}
		
		/**
		 * Get the last item in the list.
		 *@returns (value_type &) The last item in the list.
		 */
		value &last()
		{
			_hardCopy();
			return *core->pLast->pValue;
		}
		
		/**
		 * Get the last item in the list.
		 *@returns (const value_type &) The last item in the list.
		 */
		const value &last() const
		{
			return *core->pLast->pValue;
		}

		bool isEmpty() const
		{
			return (core->nSize == 0);
		}
	
	protected:
		virtual Core *_copyCore( Core *src )
		{
			Core *pRet = _allocateCore();
			for( Link *pCur = src->pFirst; pCur; pCur = pCur->pNext )
			{
				pRet->append( *pCur->pValue );
			}
			return pRet;
		}

	private:
	};

	class Formatter;
	Formatter &operator<<( Formatter &rOut, char *sStr );
	Formatter &operator<<( Formatter &rOut, signed char c );
	template<typename a, typename b, typename c>
	Formatter &operator<<( Formatter &f, const Bu::List<a,b,c> &l )
	{
		f << '[';
		for( typename Bu::List<a,b,c>::const_iterator i = l.begin(); i; i++ )
		{
			if( i != l.begin() )
				f << ", ";
			f << *i;
		}
		f << ']';

		return f;
	}

	template<typename value, typename a, typename b>
	ArchiveBase &operator<<( ArchiveBase &ar, const List<value,a,b> &h )
	{
		ar << h.getSize();
		for( typename List<value>::const_iterator i = h.begin(); i != h.end(); i++ )
		{
			ar << (*i);
		}

		return ar;
	}

	template<typename value, typename a, typename b>
	ArchiveBase &operator>>( ArchiveBase &ar, List<value,a,b> &h )
	{
		h.clear();
		long nSize;
		ar >> nSize;

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
