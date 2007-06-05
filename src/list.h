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

		MyType &operator=( const MyType &src )
		{
			clear();
			for( Link *pCur = src.pFirst; pCur; pCur = pCur->pNext )
			{
				append( *pCur->pValue );
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
			bool operator==( const iterator &oth ) const
			{
				return ( pLink == oth.pLink );
			}

			bool operator==( const Link *pOth ) const
			{
				return ( pLink == pOth );
			}

			bool operator!=( const iterator &oth ) const
			{
				return ( pLink != oth.pLink );
			}

			bool operator!=( const Link *pOth ) const
			{
				return ( pLink != pOth );
			}

			value &operator*()
			{
				return *(pLink->pValue);
			}

			value *operator->()
			{
				return pLink->pValue;
			}

			iterator &operator++()
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			iterator &operator--()
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}
			
			iterator &operator++( int )
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			iterator &operator--( int )
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}

			iterator &operator=( const iterator &oth )
			{
				pLink = oth.pLink;
				return *this;
			}
		};
		
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

		iterator begin()
		{
			return iterator( pFirst );
		}

		const const_iterator begin() const
		{
			return const_iterator( pFirst );
		}

		const Link *end() const
		{
			return NULL;
		}

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

		int getSize() const
		{
			return nSize;
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
