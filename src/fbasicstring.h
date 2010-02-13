/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_F_BASIC_STRING_H
#define BU_F_BASIC_STRING_H

#include <stdint.h>
#include <memory>

#ifndef WIN32
#include <wordexp.h>
#endif

#include "bu/util.h"
#include "bu/sharedcore.h"
#include "bu/exceptionbase.h"
#include "bu/archivebase.h"
#include "bu/list.h"

#include <string.h> 

namespace Bu
{
	template< typename chr >
	struct FStringChunk
	{
		long nLength;
		chr *pData;
		FStringChunk *pNext;
	};

#define cpy( dest, src, size ) Bu::memcpy( dest, src, size*sizeof(chr) )

	template<typename chr, int nMinSize, typename chralloc, typename chunkalloc>
	struct FStringCore
	{
		typedef struct FStringCore<chr, nMinSize, chralloc, chunkalloc> MyType;
		typedef struct FStringChunk<chr> Chunk;
		FStringCore() :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
		}

		FStringCore( const MyType &rSrc ) :
			nLength( rSrc.nLength ),
			pFirst( NULL ),
			pLast( NULL ),
			aChr( rSrc.aChr ),
			aChunk( rSrc.aChunk )
		{
			if( rSrc.pFirst == NULL || rSrc.nLength == 0 )
			{
				pFirst = pLast = NULL;
			}
			else
			{
				pFirst = pLast = newChunk( nLength );
				Chunk *pLink = rSrc.pFirst;
				int iPos = 0;
				while( pLink != NULL )
				{
					cpy( pFirst->pData+iPos, pLink->pData, pLink->nLength );
					iPos += pLink->nLength;
					pLink = pLink->pNext;
				}
			}
		}

		virtual ~FStringCore()
		{
			clear();
		}

		mutable long nLength;
		mutable Chunk *pFirst;
		mutable Chunk *pLast;

		mutable chralloc aChr;
		mutable chunkalloc aChunk;
		
		void clear() const
		{
			if( pFirst == NULL )
				return;

			Chunk *i = pFirst;
			for(;;)
			{
				Chunk *n = i->pNext;
				aChr.deallocate( i->pData, i->nLength+1 );
				aChunk.deallocate( i, 1 );
				if( n == NULL )
					break;
				i = n;
			}
			pFirst = pLast = NULL;
			nLength = 0;
		}	
		
		Chunk *newChunk() const
		{
			Chunk *pNew = aChunk.allocate( 1 );
			pNew->pNext = NULL;
			return pNew;
		}
		
		Chunk *newChunk( long nLen ) const
		{
			Chunk *pNew = aChunk.allocate( 1 );
			pNew->pNext = NULL;
			pNew->nLength = nLen;
			pNew->pData = aChr.allocate( (nLen<nMinSize)?(nMinSize):(nLen)+1 );
			pNew->pData[nLen] = (chr)0;
			return pNew;
		}

		Chunk *copyChunk( Chunk *pSrc ) const
		{
			Chunk *pNew = aChunk.allocate( 1 );
			pNew->pNext = pSrc->pNext;
			pNew->nLength = pSrc->nLength;
			pNew->pData = aChr.allocate( pSrc->nLength+1 );
			cpy( pNew->pData, pSrc->pData, pSrc->nLength );
			pNew->pData[pNew->nLength] = (chr)0;
			return pNew;
		}

		void appendChunk( Chunk *pNewChunk )
		{
			if( pFirst == NULL )
				pLast = pFirst = pNewChunk;
			else
			{
				pLast->pNext = pNewChunk;
				pLast = pNewChunk;
			}

			nLength += pNewChunk->nLength;
		}
		
		void prependChunk( Chunk *pNewChunk )
		{
			if( pFirst == NULL )
				pLast = pFirst = pNewChunk;
			else
			{
				pNewChunk->pNext = pFirst;
				pFirst = pNewChunk;
			}

			nLength += pNewChunk->nLength;
		}
	};

	/**
	 * Flexible String class.  This class was designed with string passing and
	 * generation in mind.  Like the standard string class you can specify what
	 * datatype to use for each character.  Unlike the standard string class,
	 * collection of appended and prepended terms is done lazily, making long
	 * operations that involve many appends very inexpensive.  In addition
	 * internal ref-counting means that if you pass strings around between
	 * functions there's almost no overhead in time or memory since a reference
	 * is created and no data is actually copied.  This also means that you
	 * never need to put any FBasicString into a ref-counting container class.
	 *
	 *@param chr (typename) Type of character (i.e. char)
	 *@param nMinSize (int) Chunk size (default: 256)
	 *@param chralloc (typename) Memory Allocator for chr
	 *@param chunkalloc (typename) Memory Allocator for chr chunks
	 */
	template< typename chr, int nMinSize=256, typename chralloc=std::allocator<chr>, typename chunkalloc=std::allocator<struct FStringChunk<chr> > >
	class FBasicString : public SharedCore< FStringCore<chr, nMinSize, chralloc, chunkalloc> >
	{
	protected:
		typedef struct FStringChunk<chr> Chunk;
		typedef struct FBasicString<chr, nMinSize, chralloc, chunkalloc> MyType;
		typedef struct FStringCore<chr, nMinSize, chralloc, chunkalloc> Core;

		using SharedCore< Core >::core;
		using SharedCore< Core >::_hardCopy;

	public:
		FBasicString()
		{
		}

		FBasicString( const chr *pData )
		{
			append( pData );
		}

		FBasicString( const chr *pData, long nLength )
		{
			append( pData, nLength );
		}

		FBasicString( const MyType &rSrc ) :
			SharedCore<Core>( rSrc )
		{
		}

		FBasicString( const MyType &rSrc, long nLength )
		{
			append( rSrc, nLength );
		}
		
		FBasicString( const MyType &rSrc, long nStart, long nLength )
		{
			append( rSrc, nStart, nLength );
		}

		FBasicString( long nSize )
		{
			core->pFirst = core->pLast = core->newChunk( nSize );
			core->nLength = nSize;
		}
		
		struct iterator;
		struct const_iterator
		{
			friend class FBasicString<chr, nMinSize, chralloc, chunkalloc>;
			friend struct iterator;
		private:
			const_iterator( Chunk *pChunk, int iPos ) :
				pChunk( pChunk ),
				iPos( iPos )
			{
			}

			Chunk *pChunk;
			int iPos;

		public:
			const_iterator( const const_iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}
			
			const_iterator( const iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}

			const_iterator() :
				pChunk( NULL ),
				iPos( 0 )
			{
			}

			bool operator==( const const_iterator &i ) const
			{
				return pChunk == i.pChunk && iPos == i.iPos;
			}

			bool operator!=( const const_iterator &i ) const
			{
				return !(*this == i);
			}

			const_iterator &operator=( const const_iterator &i )
			{
				pChunk = i.pChunk;
				iPos = i.iPos;
				return *this;
			}

			const_iterator &operator++()
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			const_iterator &operator++( int )
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			const_iterator &operator+=( int iAmnt )
			{
				if( !pChunk ) return *this;
				iPos += iAmnt;
				while( iPos >= pChunk->nLength )
				{
					iPos -= pChunk->nLength;
					pChunk = pChunk->pNext;
					if( pChunk == NULL )
						break;
				}
				return *this;
			}

			const_iterator operator+( int iAmnt ) const
			{
				if( !pChunk ) return *this;
				const_iterator ret( *this );
				ret += iAmnt;
				return ret;
			}

			const chr &operator *() const
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid const_iterator.");
				return pChunk->pData[iPos];
			}

			bool operator==( const chr &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] == c;
			}

			bool operator!=( const chr &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] != c;
			}

			operator bool() const
			{
				return pChunk != NULL;
			}

			bool isValid() const
			{
				return pChunk != NULL;
			}

			bool compare( const const_iterator &c ) const
			{
				const_iterator a = *this;
				const_iterator b = c;
				if( a == b )
					return true;
				for(; a && b; a++, b++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const const_iterator &c, int nLen ) const
			{
				const_iterator a = *this;
				const_iterator b = c;
				if( a == b )
					return true;
				for(int j = 0; a && b && j < nLen; a++, b++, j++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const chr *c ) const
			{
				if( !pChunk ) return false;
				const_iterator a = *this;
				for(; a && *c; a++, c++ )
				{
					if( *a != *c )
						return false;
				}
				if( a.isValid() != (*c!=(chr)0) )
					return false;
				return true;
			}

			bool compare( const chr *c, int nLen ) const
			{
				if( !pChunk ) return false;
				const_iterator a = *this;
				int j = 0;
				for(; a && j < nLen; a++, c++, j++ )
				{
					if( *a != *c )
						return false;
				}
				if( j < nLen )
					return false;
				return true;
			}

			bool compare( const MyType &s ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin() );
			}

			bool compare( const MyType &s, int nLen ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin(), nLen );
			}

			const_iterator find( const chr c ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( *i == c )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const chr *pStr, int nLen ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( pStr, nLen ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const MyType &s ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( s ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}

			const_iterator find( const MyType &s, int nLen ) const
			{
				for( const_iterator i = *this; i; i++ )
				{
					if( i.compare( s, nLen ) )
						return i;
				}
				return const_iterator( NULL, 0 );
			}
		};
		
		typedef struct iterator
		{
			friend class FBasicString<chr, nMinSize, chralloc, chunkalloc>;
			friend struct const_iterator;
		private:
			iterator( Chunk *pChunk, int iPos ) :
				pChunk( pChunk ),
				iPos( iPos )
			{
			}

			Chunk *pChunk;
			int iPos;

		public:
			iterator( const iterator &i ) :
				pChunk( i.pChunk ),
				iPos( i.iPos )
			{
			}

			iterator() :
				pChunk( NULL ),
				iPos( 0 )
			{
			}

			operator const_iterator() const
			{
				return const_iterator( pChunk, iPos );
			}

			bool operator==( const iterator &i ) const
			{
				return pChunk == i.pChunk && iPos == i.iPos;
			}

			bool operator!=( const iterator &i ) const
			{
				return !(*this == i);
			}

			iterator &operator=( const iterator &i )
			{
				pChunk = i.pChunk;
				iPos = i.iPos;
				return *this;
			}

			iterator &operator++()
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			iterator &operator++( int )
			{
				if( !pChunk ) return *this;
				iPos++;
				if( iPos >= pChunk->nLength )
				{
					iPos = 0;
					pChunk = pChunk->pNext;
				}
				return *this;
			}

			iterator &operator+=( int iAmnt )
			{
				if( !pChunk ) return *this;
				iPos += iAmnt;
				while( iPos >= pChunk->nLength )
				{
					iPos -= pChunk->nLength;
					pChunk = pChunk->pNext;
					if( pChunk == NULL )
						break;
				}
				return *this;
			}

			iterator operator+( int iAmnt ) const
			{
				if( !pChunk ) return *this;
				iterator ret( *this );
				ret += iAmnt;
				return ret;
			}

			chr &operator*()
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				return pChunk->pData[iPos];
			}

			const chr &operator*() const
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				return pChunk->pData[iPos];
			}

			bool operator==( const chr &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] == c;
			}

			bool operator!=( const chr &c ) const
			{
				if( !pChunk ) return false;
				return pChunk->pData[iPos] != c;
			}

			iterator &operator=( const chr &c )
			{
				if( !pChunk ) throw Bu::ExceptionBase("Not a valid iterator.");
				pChunk->pData[iPos] = c;
				return *this;
			}

			operator bool() const
			{
				return pChunk != NULL;
			}

			bool isValid() const
			{
				return pChunk != NULL;
			}

			bool compare( const iterator &c ) const
			{
				iterator a = *this;
				iterator b = c;
				if( a == b )
					return true;
				for(; a && b; a++, b++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const iterator &c, int nLen ) const
			{
				iterator a = *this;
				iterator b = c;
				if( a == b )
					return true;
				for(int j = 0; a && b && j < nLen; a++, b++, j++ )
				{
					if( *a != *b )
						return false;
				}
				return true;
			}

			bool compare( const chr *c ) const
			{
				if( !pChunk ) return false;
				iterator a = *this;
				for(; a && *c; a++, c++ )
				{
					if( *a != *c )
						return false;
				}
				if( a.isValid() != (*c!=(chr)0) )
					return false;
				return true;
			}

			bool compare( const chr *c, int nLen ) const
			{
				if( !pChunk ) return false;
				iterator a = *this;
				int j = 0;
				for(; a && j < nLen; a++, c++, j++ )
				{
					if( *a != *c )
						return false;
				}
				if( j < nLen )
					return false;
				return true;
			}

			bool compare( const MyType &s ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin() );
			}

			bool compare( const MyType &s, int nLen ) const
			{
				if( !pChunk ) return false;
				return compare( s.begin(), nLen );
			}

			iterator find( const chr c ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( *i == c )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const chr *pStr, int nLen ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( pStr, nLen ) )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const MyType &s ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( s ) )
						return i;
				}
				return iterator( NULL, 0 );
			}

			iterator find( const MyType &s, int nLen ) const
			{
				for( iterator i = *this; i; i++ )
				{
					if( i.compare( s, nLen ) )
						return i;
				}
				return iterator( NULL, 0 );
			}
		} iterator;

		typedef struct const_iterator const_iterator;

		//typedef chr *iterator;
//		typedef const chr *const_iterator;
		// typedef iterator const_iterator;

		FBasicString( const const_iterator &s )
		{
			append( s );
		}

		FBasicString( const const_iterator &s, const const_iterator &e )
		{
			append( s, e );
		}

		virtual ~FBasicString()
		{
		}

		/**
		 * Append data to your string.
		 *@param pData (const chr *) The data to append.
		 */
		void append( const chr *pData )
		{
			if( !pData ) return;
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ ) { }
			if( nLen == 0 )
				return;
		
			Chunk *pNew = core->newChunk( nLen );
			cpy( pNew->pData, pData, nLen );

			_hardCopy();
			core->appendChunk( pNew );
		}

		/**
		 * Append data to your string.
		 *@param pData (const chr *) The data to append.
		 *@param nLen (long) The length of the data to append.
		 */
		void append( const chr *pData, long nLen )
		{
			if( nLen == 0 )
				return;

			Chunk *pNew = core->newChunk( nLen );
			
			cpy( pNew->pData, pData, nLen );

			_hardCopy();
			core->appendChunk( pNew );
		}
		
		/**
		 * Append data to your string.
		 *@param pData (const chr *) The data to append.
		 *@param nStart (long) The start position to copy from.
		 *@param nLen (long) The length of the data to append.
		 */
		void append( const chr *pData, long nStart, long nLen )
		{
			if( nLen == 0 )
				return;

			Chunk *pNew = core->newChunk( nLen );
			
			cpy( pNew->pData, pData+nStart, nLen );

			_hardCopy();
			core->appendChunk( pNew );
		}

		/**
		 * Append a single chr to your string.
		 *@param cData (const chr &) The character to append.
		 */
		void append( const chr &cData )
		{
			if( core->pLast && core->pLast->nLength < nMinSize )
			{
				_hardCopy();
				core->pLast->pData[core->pLast->nLength] = cData;
				++core->pLast->nLength; ++core->nLength;
		//		pLast->pData[pLast->nLength] = (chr)0;
			}
			else
			{
				append( &cData, 1 );
			}
		}

		/**
		 * Append another FString to this one.
		 *@param sData (MyType &) The FString to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const MyType & sData )
		{
			append( sData.getStr(), sData.getSize() );
		}
		
		/**
		 * Append another FString to this one.
		 *@param sData (MyType &) The FString to append.
		 *@param nLen How much data to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const MyType & sData, long nLen )
		{
			append( sData.getStr(), nLen );
		}
		
		/**
		 * Append another FString to this one.
		 *@param sData (MyType &) The FString to append.
		 *@param nStart Start position in sData to start copying from.
		 *@param nLen How much data to append.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void append( const MyType & sData, long nStart, long nLen )
		{
			if( nLen < 0 )
				nLen = sData.getSize() - nStart;
			append( sData.getStr(), nStart, nLen );
		}
		
		/**
		 * Append data to this FString using the passed in iterator as a base.
		 * The iterator is const, it is not changed.
		 *@param s Iterator from any compatible FBasicString to copy data from.
		 */
		void append( const const_iterator &s )
		{
			if( !s.isValid() )
				return;
			Chunk *pSrc = s.pChunk;

			Chunk *pNew = core->newChunk( pSrc->nLength-s.iPos );
			cpy( pNew->pData, pSrc->pData+s.iPos, pSrc->nLength-s.iPos );

			_hardCopy();
			core->appendChunk( pNew );

			while( (pSrc = pSrc->pNext) )
			{
				core->appendChunk( core->copyChunk( pSrc ) );
			}
		}

		/**
		 * Append data to this FString using the passed in iterator as a base.
		 * The iterator is const, it is not changed.
		 *@param s Iterator from any compatible FBasicString to copy data from.
		 */
		void append( const iterator &s ) // I get complaints without this one
		{
			append( const_iterator( s ) );
		}

		/**
		 * Append data to this FString using the passed in iterator as a base,
		 * and copy data until the ending iterator is reached.  The character
		 * at the ending iterator is not copied.
		 * The iterators are const, they are not changed.
		 *@param s Iterator from any compatible FBasicString to copy data from.
		 *@param e Iterator to stop copying at.
		 */
		void append( const const_iterator &s, const const_iterator &e )
		{
			if( !s.isValid() )
				return;
			if( !e.isValid() )
			{
				append( s );
				return;
			}
			_hardCopy();
			if( s.pChunk == e.pChunk )
			{
				// Simple case, they're the same chunk
				Chunk *pNew = core->newChunk( e.iPos-s.iPos );
				cpy( pNew->pData, s.pChunk->pData+s.iPos, e.iPos-s.iPos );
				core->appendChunk( pNew );
			}
			else
			{
				// A little trickier, scan the blocks...
				Chunk *pSrc = s.pChunk;
				Chunk *pNew = core->newChunk( pSrc->nLength-s.iPos );
				cpy( pNew->pData, pSrc->pData+s.iPos, pSrc->nLength-s.iPos );
				core->appendChunk( pNew );

				while( (pSrc = pSrc->pNext) != e.pChunk )
				{
					core->appendChunk( core->copyChunk( pSrc ) );
				}

				pNew = core->newChunk( e.iPos );
				cpy( pNew->pData, pSrc->pData, e.iPos );
				core->appendChunk( pNew );
			}
		}
		
		/**
		 * Prepend another FString to this one.
		 *@param sData (MyType &) The FString to prepend.
		 *@todo This function can be made much faster by not using getStr()
		 */
		void prepend( const MyType & sData )
		{
			prepend( sData.getStr(), sData.getSize() );
		}

		/**
		 * Prepend data to your string.
		 *@param pData (const chr *) The data to prepend.
		 */
		void prepend( const chr *pData )
		{
			if( pData == NULL )
				return;

			_hardCopy();
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ ) { }
			
			Chunk *pNew = core->newChunk( nLen );
			cpy( pNew->pData, pData, nLen );

			core->prependChunk( pNew );
		}

		/**
		 * Prepend data to your string.
		 *@param pData (const chr *) The data to prepend.
		 *@param nLen (long) The length of the data to prepend.
		 */
		void prepend( const chr *pData, long nLen )
		{
			Chunk *pNew = core->newChunk( nLen );
			
			cpy( pNew->pData, pData, nLen );

			_hardCopy();
			core->prependChunk( pNew );
		}

		void prepend( const chr c )
		{
			prepend( &c, 1 );
		}

		/**
		 * Insert pData before byte nPos, that is, the first byte of pData will
		 * start at nPos.  This could probably be made faster by avoiding
		 * flattening.
		 */
		void insert( long nPos, const chr *pData, long nLen )
		{
			if( nLen <= 0 )
				return;
			if( nPos <= 0 )
			{
				prepend( pData, nLen );
			}
			else if( nPos >= core->nLength )
			{
				append( pData, nLen );
			}
			else
			{
				// If we're going to flatten anyway, might as well for everyone
				flatten();
				_hardCopy();
				Chunk *p1 = core->newChunk( nPos );
				Chunk *p2 = core->newChunk( nLen );
				Chunk *p3 = core->newChunk( core->nLength-nPos );
				cpy( p1->pData, core->pFirst->pData, nPos );
				cpy( p2->pData, pData, nLen );
				cpy( p3->pData, core->pFirst->pData+nPos, core->nLength-nPos );
				core->clear();
				core->appendChunk( p1 );
				core->appendChunk( p2 );
				core->appendChunk( p3 );
			}
		}

		void insert( long nPos, const MyType &str )
		{
			if( nPos <= 0 )
			{
				prepend( str );
			}
			else if( nPos >= core->nLength )
			{
				append( str );
			}
			else
			{
				flatten();
				_hardCopy();
				Chunk *p1 = core->newChunk( nPos );
				Chunk *p3 = core->newChunk( core->nLength-nPos );
				cpy( p1->pData, core->pFirst->pData, nPos );
				cpy( p3->pData, core->pFirst->pData+nPos, core->nLength-nPos );
				core->clear();
				core->appendChunk( p1 );
				for( Chunk *pChnk = str.core->pFirst; pChnk;
					 pChnk = pChnk->pNext )
				{
					core->appendChunk( core->copyChunk( pChnk ) );
				}

				core->appendChunk( p3 );
			}
		}

		/**
		 *@todo This function shouldn't use strlen, we should add our own to
		 * this class, one that can be overridden in a specific implementation.
		 */
		void insert( long nPos, const chr *pData )
		{
			insert( nPos, pData, strlen( pData ) );
		}

		void remove( long nPos, long nLen )
		{
			if( nLen <= 0 || nPos < 0 || nPos >= core->nLength )
				return;
			if( nLen > core->nLength-nPos )
				nLen = core->nLength-nPos;
			flatten();
			_hardCopy();
			cpy( core->pFirst->pData+nPos, core->pFirst->pData+nPos+nLen, core->nLength-nPos-nLen+1 );
			core->nLength -= nLen;
			core->pFirst->nLength -= nLen;
		}

		/**
		 * Clear all data from the string.
		 */
		void clear()
		{
			_hardCopy();
			core->clear();
		}

		MyType replace( const MyType &fnd, const MyType &rep ) const
		{
			MyType out;
			const_iterator o = begin();
			while( true )
			{
				const_iterator i = o.find( fnd );
				if( !i )
				{
					out.append( o );
					return out;
				}
				else
				{
					out.append( o, i );
					out.append( rep );
					o = i;
					o += fnd.getSize();
				}
			}
		}

		/**
		 * Force the string to resize
		 *@param nNewSize (long) The new size of the string.
		 */
		void resize( long nNewSize )
		{
			if( core->nLength == nNewSize )
				return;
			if( nNewSize < 0 )
				nNewSize = 0;

			flatten();
			_hardCopy();

			Chunk *pNew = core->newChunk( nNewSize );
			long nNewLen = (nNewSize<core->nLength)?(nNewSize):(core->nLength);
			if( core->nLength > 0 )
			{
				cpy( pNew->pData, core->pFirst->pData, nNewLen );
				core->aChr.deallocate( core->pFirst->pData, core->pFirst->nLength+1 );
				core->aChunk.deallocate( core->pFirst, 1 );
			}
			pNew->pData[nNewLen] = (chr)0;
			core->pFirst = core->pLast = pNew;
			core->nLength = nNewSize;
		}

		/**
		 * Get the current size of the string.
		 *@returns (long) The current size of the string.
		 */
		long getSize() const
		{
			return core->nLength;
		}
		
		/**
		 * Get a pointer to the string array.
		 *@returns (chr *) The string data.
		 */
		chr *getStr()
		{
			if( core->pFirst == NULL )
				return (chr *)"";

			flatten();
			_hardCopy();
			core->pFirst->pData[core->nLength] = (chr)0;
			return core->pFirst->pData;
		}
		
		/**
		 * Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		const chr *getStr() const
		{
			if( core->pFirst == NULL )
				return (chr *)"";

			flatten();
			core->pFirst->pData[core->nLength] = (chr)0;
			return core->pFirst->pData;
		}

		/**
		 * A convinience function, this one won't cause as much work as the
		 * non-const getStr, so if you're not changing the data, consider it.
		 */
		const chr *getConstStr() const
		{
			return getStr();
		}

		MyType getSubStrIdx( long iStart, long iSize=-1 ) const
		{
			if( iStart < 0 )
				iStart = 0;
			if( iStart >= core->nLength )
				return "";
			if( iSize < 0 )
				iSize = core->nLength;
			if( iStart+iSize > core->nLength )
				iSize = core->nLength-iStart;
			if( iSize == 0 )
				return "";

			flatten();
			MyType ret( core->pFirst->pData+iStart, iSize );
			return ret;
		}

		MyType getSubStr( const_iterator iBegin,
				const_iterator iEnd=typename MyType::const_iterator() ) const
		{
			if( !iBegin.isValid() )
				return MyType();
			if( iBegin.pChunk == iEnd.pChunk )
			{
				return MyType( iBegin.pChunk->pData+iBegin.iPos,
					iEnd.iPos-iBegin.iPos );
			}
			else if( !iEnd.isValid() )
			{
				MyType ret;
				ret.append(
					iBegin.pChunk->pData+iBegin.iPos,
					iBegin.pChunk->nLength-iBegin.iPos
					);
				for( Chunk *pCur = iBegin.pChunk->pNext;
					 pCur; pCur = pCur->pNext )
				{
					ret.append( pCur->pData, pCur->nLength );
				}
				return ret;
			}
			else
			{
				MyType ret;
				ret.append(
					iBegin.pChunk->pData+iBegin.iPos,
					iBegin.pChunk->nLength-iBegin.iPos
					);
				for( Chunk *pCur = iBegin.pChunk->pNext;
					 pCur != iEnd.pChunk; pCur = pCur->pNext )
				{
					ret.append( pCur->pData, pCur->nLength );
				}
				ret.append(
					iEnd.pChunk->pData,
					iEnd.iPos
					);
				return ret;
			}
		}

		/**
		 * (std::string compatability) Get a pointer to the string array.
		 *@returns (chr *) The string data.
		 */
		DEPRECATED
		chr *c_str()
		{
			if( core->pFirst == NULL )
				return NULL;

			flatten();
			_hardCopy();
			core->pFirst->pData[core->nLength] = (chr)0;
			return core->pFirst->pData;
		}
		
		/**
		 * (std::string compatability) Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		DEPRECATED
		const chr *c_str() const
		{
			if( core->pFirst == NULL )
				return NULL;

			flatten();
			core->pFirst->pData[core->nLength] = (chr)0;
			return core->pFirst->pData;
		}

		Bu::List<MyType> split( const chr c ) const
		{
			Bu::List<MyType> ret;
			const_iterator l, r;
			l = begin();
			for(r=l; l;)
			{
				for( r = l; r && r != c; r++ ) { }
				ret.append( MyType( l, r ) );
				l = r;
				l++;
			}
			return ret;
		}

		/**
		 * Plus equals operator for FString.
		 *@param pData (const chr *) The data to append to your FString.
		 */
		MyType &operator+=( const chr *pData )
		{
			append( pData );

			return (*this);
		}
		
		/**
		 * Plus equals operator for FString.
		 *@param rSrc (const MyType &) The FString to append to your FString.
		 */
		MyType &operator+=( const MyType &rSrc )
		{
			append( rSrc );

			return (*this);
		}

		MyType &operator+=( const MyType::const_iterator &i )
		{
			append( i, i+1 );

			return (*this);
		}

		/**
		 * Plus equals operator for FString.
		 *@param cData (const chr) The character to append to your FString.
		 */
		MyType &operator+=( const chr cData )
		{
			if( core->pLast && core->pLast->nLength < nMinSize )
			{
				_hardCopy();
				core->pLast->pData[core->pLast->nLength] = cData;
				++core->pLast->nLength; ++core->nLength;
		//		pLast->pData[pLast->nLength] = (chr)0;
			}
			else
			{
				append( &cData, 1 );
			}
			//append( pData );

			return (*this);
		}

		/**
		 * Assignment operator.
		 *@param pData (const chr *) The character array to append to your
		 *		FString.
		 */
		MyType &operator=( const chr *pData )
		{
			set( pData );

			return (*this);
		}

		MyType operator+( const MyType &rRight ) const
		{
			MyType ret( *this );
			ret.append( rRight );
			return ret;
		}

		MyType operator+( const chr *pRight ) const
		{
			MyType ret( *this );
			ret.append( pRight );
			return ret;
		}

		MyType operator+( chr *pRight ) const
		{
			MyType ret( *this );
			ret.append( pRight );
			return ret;
		}

		/**
		 * Reset your FString to this character array.
		 *@param pData (const chr *) The character array to set your FString to.
		 */
		void set( const chr *pData )
		{
			clear();
			append( pData );
		}

		/**
		 * Reset your FString to this character array.
		 *@param pData (const chr *) The character array to set your FString to.
		 *@param nSize (long) The length of the inputted character array.
		 */
		void set( const chr *pData, long nSize )
		{
			clear();
			append( pData, nSize );
		}

		void set( const chr *pData, long nStart, long nSize )
		{
			clear();
			append( pData, nStart, nSize );
		}
		
		void set( const MyType &rData )
		{
			clear();
			append( rData );
		}

		void set( const MyType &rData, long nSize )
		{
			clear();
			append( rData, nSize );
		}

		void set( const MyType &rData, long nStart, long nSize )
		{
			clear();
			append( rData, nStart, nSize );
		}

		void set( const_iterator s )
		{
			clear();
			append( s );
		}

		void set( const_iterator s, const_iterator e )
		{
			clear();
			append( s, e );
		}

		/**
		 * Resize the string, possibly to make room for a copy.  At the moment
		 * this operation *is* destructive.  What was in the string will in no
		 * way be preserved.  This is, however, very fast.  If you want to
		 * keep your data check out resize.
		 *@param iSize the new size in bytes.  The string is guranteed to have
		 * at least this much contiguous space available when done.
		 */
		void setSize( long iSize )
		{
			_hardCopy();
			core->clear();
			core->appendChunk( core->newChunk( iSize ) );
		}

		void expand()
		{
#ifndef WIN32
			flatten();
			_hardCopy();

			wordexp_t result;

			/* Expand the string for the program to run.  */
			switch (wordexp (core->pFirst->pData, &result, 0))
			{
				case 0:                       /* Successful.  */
					{
						set( result.we_wordv[0] );
						wordfree( &result );
						return;
					}
					break;
				case WRDE_NOSPACE:
					/* If the error was `WRDE_NOSPACE',
					then perhaps part of the result was allocated.  */
					wordfree (&result);
				default:                    /* Some other error.  */
					return;
			}
#endif
		}

		/**
		 * Assignment operator.
		 *@param rSrc (const MyType &) The FString to set your FString to.
		 */
	/*	MyType &operator=( const MyType &rSrc )
		{
			set( rSrc );

			return (*this);
		} */
		
		/**
		 * Equals comparison operator.
		 *@param pData (const chr *) The character array to compare your FString
		 *		to.
		 */
		bool operator==( const chr *pData ) const
		{
			if( core->pFirst == NULL ) {
				if( pData == NULL )
					return true;
				if( pData[0] == (chr)0 )
					return true;
				return false;
			}

			flatten();
			core->pFirst->pData[core->nLength] = (chr)0;
			const chr *a = pData;
			chr *b = core->pFirst->pData;
			for( long j = 0; *a!=(chr)0 || *b!=(chr)0; j++, a++, b++ )
			{
				if( *a != *b )
					return false;
				if( *a == (chr)0 && j < core->nLength )
					return false;
			}

			return true;
		}
		
		/**
		 * Equals comparison operator.
		 *@param pData (const MyType &) The FString to compare your FString to.
		 */
		bool operator==( const MyType &pData ) const
		{
			if( core == pData.core )
				return true;
			if( core->pFirst == pData.core->pFirst )
				return true;
			if( (core->pFirst == 0 && pData.core->nLength == 0) ) 
				return true;
			if( core->nLength != pData.core->nLength )
				return false;

			flatten();
			pData.flatten();
			const chr *a = pData.core->pFirst->pData;
			chr *b = core->pFirst->pData;
			for( long j = 0; j < core->nLength; j++, a++, b++ )
			{
				if( *a != *b )
					return false;
			}

			return true;
		}

		/**
		 * Not equals comparison operator.
		 *@param pData (const chr *) The character array to compare your FString
		 *		to.
		 */
		bool operator!=(const chr *pData ) const
		{
			return !(*this == pData);
		}
		
		/**
		 * Not equals comparison operator.
		 *@param pData (const MyType &) The FString to compare your FString to.
		 */
		bool operator!=(const MyType &pData ) const
		{
			return !(*this == pData);
		}

		bool operator<(const MyType &pData ) const
		{
			flatten();
			pData.flatten();

			const chr *a = core->pFirst->pData;
			chr *b = pData.core->pFirst->pData;
			for( long j = 0; j < core->nLength; j++, a++, b++ )
			{
				if( *a != *b )
					return *a < *b;
			}

			return false;
		}

		bool operator>(const MyType &pData ) const
		{
			flatten();
			pData.flatten();

			const chr *a = core->pFirst->pData;
			chr *b = pData.core->pFirst->pData;
			for( long j = 0; j < core->nLength; j++, a++, b++ )
			{
				if( *a != *b )
					return *a > *b;
			}

			return false;
		}

		/**
		 * Indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (chr &) The character at position (nIndex).
		 */
		chr &operator[]( long nIndex )
		{
			if( nIndex < 0 || nIndex >= core->nLength )
				throw Bu::ExceptionBase("Index out of range.");
			flatten();
			_hardCopy();

			return core->pFirst->pData[nIndex];
		}
		
		/**
		 * Const indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (const chr &) The character at position (nIndex).
		 */
		const chr &operator[]( long nIndex ) const
		{
			if( nIndex < 0 || nIndex >= core->nLength )
				throw Bu::ExceptionBase("Index out of range.");
			flatten();

			return core->pFirst->pData[nIndex];
		}
/*
		operator const chr *() const
		{
			if( !pFirst ) return NULL;
			flatten();
			return pFirst->pData;
		}
		*/

		operator bool() const
		{
			return (core->pFirst != NULL);
		}

		bool isSet() const
		{
			return (core->pFirst != NULL);
		}

		bool compareSub( const chr *pData, long nIndex, long nLen ) const
		{
			if( core->pFirst == NULL ) {
				if( pData == NULL )
					return true;
				if( pData[0] == (chr)0 )
					return true;
				return false;
			}
			if( nIndex+nLen > core->nLength )
				return false;

			flatten();
			core->pFirst->pData[core->nLength] = (chr)0;
			const chr *a = pData;
			chr *b = core->pFirst->pData+nIndex;
			for( long j = 0; j < nLen; j++, a++, b++ )
			{
				if( *a != *b )
					return false;
				if( *a == (chr)0 && j < core->nLength )
					return false;
			}

			return true;
		}

		bool compareSub( const MyType &rData, long nIndex, long nLen ) const
		{
			if( core->pFirst == NULL || rData.core->pFirst == NULL ) 
				return false;
			if( nLen < 0 )
				nLen = rData.core->nLength;
			if( nIndex+nLen > core->nLength )
				return false;

			flatten();
			rData.flatten();
			const chr *a = rData.core->pFirst->pData;
			chr *b = core->pFirst->pData + nIndex;
			for( long j = 0; j < nLen; j++, a++, b++ )
			{
				if( *a != *b )
					return false;
			}

			return true;
		}
		
		/**
		 * Is the character at index (nIndex) white space?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it white space?
		 */
		bool isWS( long nIndex ) const
		{
			flatten();

			return core->pFirst->pData[nIndex]==' ' || core->pFirst->pData[nIndex]=='\t'
				|| core->pFirst->pData[nIndex]=='\r' || core->pFirst->pData[nIndex]=='\n';
		}

		/**
		 * Is the character at index (nIndex) a letter?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it a letter?
		 */
		bool isAlpha( long nIndex ) const
		{
			flatten();

			return (core->pFirst->pData[nIndex] >= 'a' && core->pFirst->pData[nIndex] <= 'z')
				|| (core->pFirst->pData[nIndex] >= 'A' && core->pFirst->pData[nIndex] <= 'Z');
		}

		/**
		 * Convert your alpha characters to lower case.
		 */
		void toLower()
		{
			flatten();
			_hardCopy();

			for( long j = 0; j < core->nLength; j++ )
			{
				if( core->pFirst->pData[j] >= 'A' && core->pFirst->pData[j] <= 'Z' )
					core->pFirst->pData[j] -= 'A'-'a';
			}
		}

		/**
		 * Convert your alpha characters to upper case.
		 */
		void toUpper()
		{
			flatten();
			_hardCopy();

			for( long j = 0; j < core->nLength; j++ )
			{
				if( core->pFirst->pData[j] >= 'a' && core->pFirst->pData[j] <= 'z' )
					core->pFirst->pData[j] += 'A'-'a';
			}
		}

//		template<typename out>
//		void to( out &dst );
/*		{
			flatten();

			dst = strtol( pFirst->pData, NULL, 0 );
		} */

		const_iterator find( const chr cChar,
				const_iterator iStart=typename MyType::const_iterator() ) const
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( cChar == *iStart )
					return iStart;
			}
			return end();
		}

		const_iterator find( const chr *sText, int nLen,
				const_iterator iStart=typename MyType::const_iterator() ) const
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( sText, nLen ) )
					return iStart;
			}
			return end();
		}

		const_iterator find( const MyType &rStr,
				const_iterator iStart=typename MyType::const_iterator() ) const
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( rStr ) )
					return iStart;
			}
			return end();
		}

		const_iterator find( const MyType &rStr, int nLen,
				const_iterator iStart=typename MyType::const_iterator() ) const
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( rStr, nLen ) )
					return iStart;
			}
			return end();
		}

		iterator find( const chr cChar,
				const_iterator iStart=typename MyType::const_iterator() )
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( cChar == *iStart )
					return iterator( iStart.pChunk, iStart.iPos );
			}
			return end();
		}

		iterator find( const chr *sText, int nLen,
				const_iterator iStart=typename MyType::const_iterator() )
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( sText, nLen ) )
					return iterator( iStart.pChunk, iStart.iPos );
			}
			return end();
		}

		iterator find( const MyType &rStr,
				const_iterator iStart=typename MyType::const_iterator() )
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( rStr ) )
					return iterator( iStart.pChunk, iStart.iPos );
			}
			return end();
		}

		iterator find( const MyType &rStr, int nLen,
				const_iterator iStart=typename MyType::const_iterator() )
		{
			if( !iStart ) iStart = begin();
			for( ; iStart; iStart++ )
			{
				if( iStart.compare( rStr, nLen ) )
					return iterator( iStart.pChunk, iStart.iPos );
			}
			return end();
		}

		/**
		 * Find the index of the first occurrance of cChar
		 *@param cChar The character to search for.
		 *@param iStart The position in the string to start searching from.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long findIdx( const chr cChar, long iStart=0 ) const
		{
			flatten();
			for( long j = iStart; j < core->pFirst->nLength; j++ )
			{
				if( core->pFirst->pData[j] == cChar )
					return j;
			}
			return -1;
		}
		
		/**
		 * Find the index of the first occurrance of sText
		 *@param sText The null-terminated string to search for.
		 *@param iStart The position in the string to start searching from.
		 *@returns The index of the first occurrance. -1 for not found.
		 */
		long findIdx( const chr *sText, long iStart=0 ) const
		{
			long nTLen = strlen( sText );
			flatten();
			for( long j = iStart; j < core->pFirst->nLength-nTLen; j++ )
			{
				if( !strncmp( sText, core->pFirst->pData+j, nTLen ) )
					return j;
			}
			return -1;
		}

		/**
		 * Do a reverse search for (sText)
		 *@param sText (const chr *) The string to search for.
		 *@returns (long) The index of the last occurrance. -1 for not found.
		 */
		long rfindIdx( const chr *sText ) const
		{
			long nTLen = strlen( sText );
			flatten();
			for( long j = core->pFirst->nLength-nTLen-1; j >= 0; j-- )
			{
				if( !strncmp( sText, core->pFirst->pData+j, nTLen ) )
					return j;
			}
			return -1;
		}

		/**
		 * Remove nAmnt bytes from the front of the string.  This function
		 * operates in O(n) time and should be used sparingly.
		 */
		void trimFront( long nAmnt )
		{
			long nNewLen = core->nLength - nAmnt;
			flatten();
			Chunk *pNew = core->newChunk( nNewLen );
			cpy( pNew->pData, core->pFirst->pData+nAmnt, nNewLen );
			_hardCopy();
			core->clear();
			core->appendChunk( pNew );
		}

		void trimBack( chr c )
		{
			if( core->pFirst == NULL )
				return;
			flatten();
			for( ; core->pFirst->nLength > 0 && core->pFirst->pData[core->pFirst->nLength-1] == c; core->pFirst->nLength--, core->nLength-- ) { }
		}

		void format( const char *sFrmt, ...)
		{
			_hardCopy();
			clear();

			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );
			
			Chunk *pNew = core->newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			core->appendChunk( pNew );

			va_end( ap );
		}

		void formatAppend( const char *sFrmt, ...)
		{
			_hardCopy();
			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );

			Chunk *pNew = core->newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			core->appendChunk( pNew );

			va_end( ap );
		}

		void formatPrepend( const char *sFrmt, ...)
		{
			_hardCopy();
			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );
			
			Chunk *pNew = core->newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			core->prependChunk( pNew );

			va_end( ap );
		}

		iterator begin()
		{
			if( core->nLength == 0 )
				return iterator( NULL, 0 );
			return iterator( core->pFirst, 0 );
		}

		const_iterator begin() const
		{
			if( core->nLength == 0 )
				return const_iterator( NULL, 0 );
			return iterator( core->pFirst, 0 );
		}

		iterator end()
		{
			return iterator( NULL, 0 );
		}

		const_iterator end() const
		{
			return const_iterator( NULL, 0 );
		}

		bool isEmpty() const
		{
			if( core->nLength == 0 )
				return true;
			return false;
		}

	private:
		void flatten() const
		{
			if( isFlat() )
				return;

			if( core->pFirst == NULL )
				return;

			Chunk *pNew = core->newChunk( core->nLength );
			chr *pos = pNew->pData;
			Chunk *i = core->pFirst;
			for(;;)
			{
				cpy( pos, i->pData, i->nLength );
				pos += i->nLength;
				i = i->pNext;
				if( i == NULL )
					break;
			}
			core->clear();

			core->pLast = core->pFirst = pNew;
			core->nLength = pNew->nLength;
		}
		
		bool isFlat() const
		{
			return (core->pFirst == core->pLast);
		}
	};
	
	template<class T> FBasicString<T> operator+( const T *pLeft, const FBasicString<T> &rRight )
	{
		Bu::FBasicString<T> ret( pLeft );
		ret.append( rRight );
		return ret;
	}

	template<class chr, int b, class c, class d>
	ArchiveBase &operator<<( ArchiveBase &ar, const FBasicString<chr, b, c, d> &s )
	{
		long n = s.getSize();
		ar << n;
		ar.write( s.getConstStr(), n );
		return ar;
	}

	template<class chr, int b, class c, class d>
	ArchiveBase &operator>>( ArchiveBase &ar, FBasicString<chr, b, c, d> &s )
	{
		long n;
		ar >> n;
		s.setSize( n );
		ar.read( s.getStr(), n );
		return ar;
	}
}

#undef cpy

#endif
