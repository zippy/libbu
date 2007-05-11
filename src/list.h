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
	template<typename value, typename valuealloc=std::allocator<value>, typename linkalloc=std::allocator<struct ListLink<value> > >
	class List
	{
	private:
		typedef struct ListLink<value> Link;
		typedef class List<value, valuealloc, linkalloc> MyType;

	public:
		List() :
			pFirst( NULL ),
			pLast( NULL )
		{
		}

		void append( value v )
		{
			Link *pNew = la.allocate( sizeof( Link ) );
			pNew->pValue = va.allocate( sizeof( value ) );
			va.construct( pNew->pValue, v );
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

		void prepend( value v )
		{
			Link *pNew = la.allocate( sizeof( Link ) );
			pNew->pValue = va.allocate( sizeof( value ) );
			va.construct( pNew->pValue, v );
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
			bool operator==( const iterator &oth )
			{
				return ( pLink == oth.pLink );
			}

			bool operator==( const Link *pOth )
			{
				return ( pLink == pOth );
			}

			bool operator!=( const iterator &oth )
			{
				return ( pLink != oth.pLink );
			}

			bool operator!=( const Link *pOth )
			{
				return ( pLink != pOth );
			}

			value &operator*()
			{
				return *(pLink->pValue);
			}

			value *operator->()
			{
				return pLink->pValue();
			}

			iterator operator++()
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			iterator operator--()
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}
			
			iterator operator++( int )
			{
				if( pLink != NULL )
					pLink = pLink->pNext;
				return *this;
			}

			iterator operator--( int )
			{
				if( pLink != NULL )
					pLink = pLink->pPrev;
				return *this;
			}

			iterator operator=( const iterator &oth )
			{
				pLink = oth.pLink;
			}
		};

		iterator begin()
		{
			return iterator( pFirst );
		}

		const Link *end()
		{
			return NULL;
		}

		int getSize()
		{
			int j = 0;
			for( Link *pCur = pFirst; pCur; pCur = pCur->pNext )
				j++;
			return j;
		}
		
	private:
		Link *pFirst;
		Link *pLast;
		linkalloc la;
		valuealloc va;
	};
}

#endif
