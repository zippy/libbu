#ifndef LIST_H
#define LIST_H

#include <memory>
#include "bu/exceptionbase.h"

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
	 */
	template<typename value, typename valuealloc=std::allocator<value>, typename linkalloc=std::allocator<struct ListLink<value> > >
	class List
	{
	private:
		typedef struct ListLink<value> Link;
		typedef class List<value, valuealloc, linkalloc> MyType;

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
		 * An iterator to iterate through your list.
		 */
		typedef struct iterator
		{
			friend class List<value, valuealloc, linkalloc>;
		private:
			Link *pLink;
			iterator() :
				pLink( NULL )
			{
			}

			iterator( Link *pLink ) :
				pLink( pLink )
			{
			}

		public:
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

			/**
			 * Increment operator.
			 */
			iterator &operator++()
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			/**
			 * Decrement operator.
			 */
			iterator &operator--()
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}
			
			/**
			 * Increment operator.
			 */
			iterator &operator++( int )
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			/**
			 * Decrement operator.
			 */
			iterator &operator--( int )
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
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
		};
		
		/**
		 *@see iterator
		 */
		typedef struct const_iterator
		{
			friend class List<value, valuealloc, linkalloc>;
		private:
			Link *pLink;
			const_iterator() :
				pLink( NULL )
			{
			}

			const_iterator( Link *pLink ) :
				pLink( pLink )
			{
			}

			const_iterator( const iterator &i ) :
				pLink( i.pLink )
			{
			}

		public:
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
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			const_iterator &operator--()
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}
			
			const_iterator &operator++( int )
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			const_iterator &operator--( int )
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
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
		};

		/**
		 * Get an iterator pointing to the first item in the list.
		 *@returns (iterator)
		 */
		iterator begin()
		{
			return iterator( pFirst );
		}

		/**
		 * Get a const iterator pointing to the first item in the list.
		 *@returns (const const_iterator)
		 */
		const const_iterator begin() const
		{
			return const_iterator( pFirst );
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
		void erase( iterator &i )
		{
			Link *pCur = i.pLink;
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
				nSize--;
				i.pLink = pFirst;
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
				i.pLink = pTmp;
			}
		}

		/**
		 * Get the current size of the list.
		 *@returns (int) The current size of the list.
		 */
		int getSize() const
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
		
	private:
		Link *pFirst;
		Link *pLast;
		linkalloc la;
		valuealloc va;
		int nSize;
	};
}

#endif
