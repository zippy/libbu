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
#include "bu/util.h"

namespace Bu
{
	template<typename value>
	struct ListLink
	{
		value *pValue;
		ListLink *pNext;
		ListLink *pPrev;
	};

	/**
	 * Linked list template container.  This class is similar to the stl list
	 * class except for a few minor changes.  First, it doesn't mimic a stack or
	 * queue, use the Stack or Queue clasess for that.  Second, when const, all
	 * members are only accessable const.  Third, erasing a location does not
	 * invalidate the iterator, it simply points to the next valid location, or
	 * end() if there are no more.
	 *
	 *@param value (typename) The type of data to store in your list
	 *@param valuealloc (typename) Memory Allocator for your value type
	 *@param linkalloc (typename) Memory Allocator for the list links.
	 *@ingroup Containers
	 */
	template<typename value, typename cmpfunc=__basicGTCmp<value>,
		typename valuealloc=std::allocator<value>,
		typename linkalloc=std::allocator<struct ListLink<value> > >
	class List
	{
	private:
		typedef struct ListLink<value> Link;
		typedef class List<value, cmpfunc, valuealloc, linkalloc> MyType;

	public:
		List() :
			pFirst( NULL ),
			pLast( NULL ),
			nSize( 0 )
		{
		}

		List( const MyType &src ) :
			pFirst( NULL ),
			pLast( NULL ),
			nSize( 0 )
		{
			for( Link *pCur = src.pFirst; pCur; pCur = pCur->pNext )
			{
				append( *pCur->pValue );
			}
		}

		~List()
		{
			clear();
		}

		/**
		 * Assignment operator.
		 *@param src (const MyType &) The list to assign to your list.
		 */
		MyType &operator=( const MyType &src )
		{
			clear();
			for( Link *pCur = src.pFirst; pCur; pCur = pCur->pNext )
			{
				append( *pCur->pValue );
			}
			return *this;
		}

		/**
		 * Clear the data from the list.
		 */
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

		void enqueue( const value &v )
		{
			append( v );
		}

		value dequeue()
		{
			value v = *pFirst->pValue;

			erase( begin() );

			return v;
		}


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

		/**
		 * Insert a new item in sort order by searching for the first item that
		 * is larger and inserting this before it, or at the end if none are
		 * larger.  If this is the only function used to insert data in the
		 * List all items will be sorted.  To use this, the value type must
		 * support the > operator.
		 */
		void insertSorted( const value &v )
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
				return;
			}
			else
			{
				Link *pCur = pFirst;
				for(;;)
				{
					if( !cmp( v, *(pCur->pValue)) )
					{
						pNew->pNext = pCur;
						pNew->pPrev = pCur->pPrev;
						pCur->pPrev = pNew;
						if( pNew->pPrev == NULL )
							pFirst = pNew;
						else
							pNew->pPrev->pNext = pNew;
						return;
					}
					pCur = pCur->pNext;
					if( pCur == NULL )
					{
						pNew->pNext = NULL;
						pNew->pPrev = pLast;
						pLast->pNext = pNew;
						pLast = pNew;
						return;
					}
				}
			}
		}

		/**
		 * An iterator to iterate through your list.
		 */
		typedef struct iterator
		{
			friend class List<value, cmpfunc, valuealloc, linkalloc>;
		private:
			Link *pLink;
			MyType &rList;
			bool bOffFront;
			iterator( MyType &rList ) :
				pLink( NULL ),
				rList( rList )
			{
			}

			iterator( Link *pLink, MyType &rList ) :
				pLink( pLink ),
				rList( rList )
			{
			}

		public:
			iterator( const iterator &i ) :
				pLink( i.pLink ),
				rList( i.rList )
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
					pLink = (bOffFront)?(rList.pFirst):(NULL);
				else
					pLink = pLink->pNext;
				bOffFront = false;
				return *this;
			}

			iterator &operator--()
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(NULL):(rList.pLast);
				else
					pLink = pLink->pPrev;
				bOffFront = true;
				return *this;
			}
			
			iterator &operator++( int )
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(rList.pFirst):(NULL);
				else
					pLink = pLink->pNext;
				bOffFront = false;
				return *this;
			}

			iterator &operator--( int )
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(NULL):(rList.pLast);
				else
					pLink = pLink->pPrev;
				bOffFront = true;
				return *this;
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
			friend class List<value, cmpfunc, valuealloc, linkalloc>;
		private:
			Link *pLink;
			const MyType &rList;
			bool bOffFront;
			const_iterator( const MyType &rList ) :
				pLink( NULL ),
				rList( rList )
			{
			}

			const_iterator( Link *pLink, const MyType &rList ) :
				pLink( pLink ),
				rList( rList )
			{
			}

		public:
			const_iterator( const iterator &i ) :
				pLink( i.pLink ),
				rList( i.rList )
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
					pLink = (bOffFront)?(rList.pFirst):(NULL);
				else
					pLink = pLink->pNext;
				bOffFront = false;
				return *this;
			}

			const_iterator &operator--()
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(NULL):(rList.pLast);
				else
					pLink = pLink->pPrev;
				bOffFront = true;
				return *this;
			}
			
			const_iterator &operator++( int )
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(rList.pFirst):(NULL);
				else
					pLink = pLink->pNext;
				bOffFront = false;
				return *this;
			}

			const_iterator &operator--( int )
			{
				if( pLink == NULL )
					pLink = (bOffFront)?(NULL):(rList.pLast);
				else
					pLink = pLink->pPrev;
				bOffFront = true;
				return *this;
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
		} const_iterator;

		/**
		 * Get an iterator pointing to the first item in the list.
		 *@returns (iterator)
		 */
		iterator begin()
		{
			return iterator( pFirst, *this );
		}

		/**
		 * Get a const iterator pointing to the first item in the list.
		 *@returns (const const_iterator)
		 */
		const_iterator begin() const
		{
			return const_iterator( pFirst, *this );
		}

		/**
		 * Get an iterator pointing to a place just past the last item in
		 * 		the list.
		 *@returns (const Link *)
		 */
		const Link *end() const
		{
			return NULL;
		}

		/**
		 * Erase an item from the list.
		 *@param i (iterator) The item to erase.
		 */
		void erase( iterator i )
		{
			Link *pCur = i.pLink;
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

		/**
		 * Erase an item from the list if you already know the item.
		 *@param ob The item to find and erase.
		 */
		void erase( const value &v )
		{
			for( iterator i = begin(); i != end(); i++ )
			{
				if( (*i) == v )
				{
					erase( i );
					return;
				}
			}
		}

		/**
		 * Get the current size of the list.
		 *@returns (int) The current size of the list.
		 */
		long getSize() const
		{
			return nSize;
		}

		/**
		 * Get the first item in the list.
		 *@returns (value_type &) The first item in the list.
		 */
		value &first()
		{
			return *pFirst->pValue;
		}
		
		/**
		 * Get the first item in the list.
		 *@returns (const value_type &) The first item in the list.
		 */
		const value &first() const
		{
			return *pFirst->pValue;
		}
		
		/**
		 * Get the last item in the list.
		 *@returns (value_type &) The last item in the list.
		 */
		value &last()
		{
			return *pLast->pValue;
		}
		
		/**
		 * Get the last item in the list.
		 *@returns (const value_type &) The last item in the list.
		 */
		const value &last() const
		{
			return *pLast->pValue;
		}

		bool isEmpty() const
		{
			return (nSize == 0);
		}
		
	private:
		Link *pFirst;
		Link *pLast;
		linkalloc la;
		valuealloc va;
		long nSize;
		cmpfunc cmp;
	};
}

#endif
