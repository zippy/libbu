#ifndef F_STRING_H
#define F_STRING_H

#include <stdint.h>
#include <memory>
#include "bu/archival.h"
#include "bu/archive.h"
#include "bu/hash.h"

#define min( a, b )  ((a<b)?(a):(b))

namespace Bu
{
	template< typename chr >
	struct FStringChunk
	{
		long nLength;
		chr *pData;
		FStringChunk *pNext;
	};

	/**
	 * Flexible String class.  This class was designed with string passing and
	 * generation in mind.  Like the standard string class you can specify what
	 * datatype to use for each character.  Unlike the standard string class,
	 * collection of appended and prepended terms is done lazily, making long
	 * operations that involve many appends very inexpensive.  In addition internal
	 * ref-counting means that if you pass strings around between functions there's
	 * almost no overhead in time or memory since a reference is created and no
	 * data is actually copied.  This also means that you never need to put any
	 * FBasicString into a ref-counting container class.
	 *
	 *@param chr (typename) Type of character (i.e. char)
	 *@param nMinSize (int) Chunk size (default: 256)
	 *@param chralloc (typename) Memory Allocator for chr
	 *@param chunkalloc (typename) Memory Allocator for chr chunks
	 */
	template< typename chr, int nMinSize=256, typename chralloc=std::allocator<chr>, typename chunkalloc=std::allocator<struct FStringChunk<chr> > >
	class FBasicString : public Archival
	{
#ifndef VALTEST
#define cpy( dest, src, size ) memcpy( dest, src, size*sizeof(chr) )
#endif
	private:
		typedef struct FStringChunk<chr> Chunk;
		typedef struct FBasicString<chr, nMinSize, chralloc, chunkalloc> MyType;

	public:
		FBasicString() :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
		}

		FBasicString( const chr *pData ) :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( pData );
		}

		FBasicString( const chr *pData, long nLength ) :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( pData, nLength );
		}

		FBasicString( const MyType &rSrc ) :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			// Here we have no choice but to copy, since the other guy is a const.
			// In the case that the source were flat, we could get a reference, it
			// would make some things faster, but not matter in many other cases.

			joinShare( rSrc );
			//copyFrom( rSrc );
		}

		FBasicString( const MyType &rSrc, long nLength ) :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( rSrc.pFirst->pData, nLength );
		}
		
		FBasicString( const MyType &rSrc, long nStart, long nLength ) :
			nLength( 0 ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( rSrc.pFirst->pData+nStart, nLength );
		}

		FBasicString( long nSize ) :
			nLength( nSize ),
			pnRefs( NULL ),
			pFirst( NULL ),
			pLast( NULL )
		{
			pFirst = pLast = newChunk( nSize );
		}

		virtual ~FBasicString()
		{
			clear();
		}

		/**
		 *@todo void append( const MyType & sData )
		 */

		/**
		 * Append data to your string.
		 *@param pData (const chr *) The data to append.
		 */
		void append( const chr *pData )
		{
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
			if( nLen == 0 )
				return;
			
			Chunk *pNew = newChunk( nLen );
			cpy( pNew->pData, pData, nLen );

			appendChunk( pNew );
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

			Chunk *pNew = newChunk( nLen );
			
			cpy( pNew->pData, pData, nLen );

			appendChunk( pNew );
		}

		/**
		 * Append a single chr to your string.
		 *@param cData (const chr &) The character to append.
		 */
		void append( const chr &cData )
		{
			append( &cData, 1 );
		}

		/**
		 * Prepend another FString to this one.
		 *@param sData (MyType &) The FString to prepend.
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
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
			
			Chunk *pNew = newChunk( nLen );
			cpy( pNew->pData, pData, nLen );

			prependChunk( pNew );
		}

		/**
		 * Prepend data to your string.
		 *@param pData (const chr *) The data to prepend.
		 *@param nLen (long) The length of the data to prepend.
		 */
		void prepend( const chr *pData, long nLen )
		{
			Chunk *pNew = newChunk( nLen );
			
			cpy( pNew->pData, pData, nLen );

			prependChunk( pNew );
		}

		/**
		 *@todo void prepend( const chr &cData )
		 */

		/**
		 * Clear all data from the string.
		 */
		void clear()
		{
			realClear();
		}

		/**
		 * Force the string to resize
		 *@param nNewSize (long) The new size of the string.
		 */
		void resize( long nNewSize )
		{
			if( nLength == nNewSize )
				return;

			flatten();

			Chunk *pNew = newChunk( nNewSize );
			long nNewLen = (nNewSize<nLength)?(nNewSize):(nLength);
			cpy( pNew->pData, pFirst->pData, nNewLen );
			pNew->pData[nNewLen] = (chr)0;
			aChr.deallocate( pFirst->pData, pFirst->nLength+1 );
			aChunk.deallocate( pFirst, 1 );
			pFirst = pLast = pNew;
			nLength = nNewSize;
		}

		/**
		 * Get the current size of the string.
		 *@returns (long) The current size of the string.
		 */
		long getSize() const
		{
			return nLength;
		}
		
		/**
		 * Get a pointer to the string array.
		 *@returns (chr *) The string data.
		 */
		chr *getStr()
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			return pFirst->pData;
		}
		
		/**
		 * Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		const chr *getStr() const
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			return pFirst->pData;
		}

		/**
		 * (std::string compatability) Get a pointer to the string array.
		 *@returns (chr *) The string data.
		 */
		chr *c_str()
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			return pFirst->pData;
		}
		
		/**
		 * (std::string compatability) Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		const chr *c_str() const
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			return pFirst->pData;
		}

		/**
		 * Plus equals operator for FString.
		 *@param pData (const chr *) The data to append to your FString.
		 */
		MyType &operator +=( const chr *pData )
		{
			append( pData );

			return (*this);
		}
		
		/**
		 * Plus equals operator for FString.
		 *@param pData (const MyType &) The FString to append to your FString.
		 */
		MyType &operator +=( const MyType &rSrc )
		{
			if( rSrc.nLength == 0 )
				return (*this);
			rSrc.flatten();
			append( rSrc.pFirst->pData, rSrc.nLength );

			return (*this);
		}

		/**
		 * Plus equals operator for FString.
		 *@param pData (const chr) The character to append to your FString.
		 */
		MyType &operator +=( const chr pData )
		{
			append( &pData, 1 );

			return (*this);
		}

		/**
		 * Assignment operator.
		 *@param pData (const chr *) The character array to append to your
		 *		FString.
		 */
		MyType &operator =( const chr *pData )
		{
			clear();
			append( pData );

			return (*this);
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

		/**
		 * Assignment operator.
		 *@param rSrc (const MyType &) The FString to set your FString to.
		 */
		MyType &operator =( const MyType &rSrc )
		{
			//if( rSrc.isFlat() )
			//{
				joinShare( rSrc );
			//}
			//else
			//{
			//	copyFrom( rSrc );
			//}
			//

			return (*this);
		}
		
		/**
		 * Equals comparison operator.
		 *@param pData (const chr *) The character array to compare your FString
		 *		to.
		 */
		bool operator ==( const chr *pData ) const
		{
			if( pFirst == NULL ) {
				if( pData == NULL )
					return true;
				return false;
			}

			flatten();
			const chr *a = pData;
			chr *b = pFirst->pData;
			for( ; *a!=(chr)0; a++, b++ )
			{
				if( *a != *b )
					return false;
			}

			return true;
		}
		
		/**
		 * Equals comparison operator.
		 *@param pData (const MyType &) The FString to compare your FString to.
		 */
		bool operator ==( const MyType &pData ) const
		{
			if( pFirst == pData.pFirst )
				return true;
			if( pFirst == NULL ) 
				return false;

			flatten();
			pData.flatten();
			const chr *a = pData.pFirst->pData;
			chr *b = pFirst->pData;
			for( ; *a!=(chr)0; a++, b++ )
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
		bool operator !=(const chr *pData ) const
		{
			return !(*this == pData);
		}
		
		/**
		 * Not equals comparison operator.
		 *@param pData (const MyType &) The FString to compare your FString to.
		 */
		bool operator !=(const MyType &pData ) const
		{
			return !(*this == pData);
		}

		/**
		 * Indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (chr &) The character at position (nIndex).
		 */
		chr &operator[]( long nIndex )
		{
			flatten();

			return pFirst->pData[nIndex];
		}
		
		/**
		 * Const indexing operator
		 *@param nIndex (long) The index of the character you want.
		 *@returns (const chr &) The character at position (nIndex).
		 */
		const chr &operator[]( long nIndex ) const
		{
			flatten();

			return pFirst->pData[nIndex];
		}

		/**
		 * Is the character at index (nIndex) white space?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it white space?
		 */
		bool isWS( long nIndex ) const
		{
			flatten();

			return pFirst->pData[nIndex]==' ' || pFirst->pData[nIndex]=='\t'
				|| pFirst->pData[nIndex]=='\r' || pFirst->pData[nIndex]=='\n';
		}

		/**
		 * Is the character at index (nIndex) a letter?
		 *@param nIndex (long) The index of the character you want to check.
		 *@returns (bool) Is it a letter?
		 */
		bool isAlpha( long nIndex ) const
		{
			flatten();

			return (pFirst->pData[nIndex] >= 'a' && pFirst->pData[nIndex] <= 'z')
				|| (pFirst->pData[nIndex] >= 'A' && pFirst->pData[nIndex] <= 'Z');
		}

		/**
		 * Convert your alpha characters to lower case.
		 */
		void toLower()
		{
			flatten();
			unShare();

			for( long j = 0; j < nLength; j++ )
			{
				if( pFirst->pData[j] >= 'A' && pFirst->pData[j] <= 'Z' )
					pFirst->pData[j] -= 'A'-'a';
			}
		}

		/**
		 * Convert your alpha characters to upper case.
		 */
		void toUpper()
		{
			flatten();
			unShare();

			for( long j = 0; j < nLength; j++ )
			{
				if( pFirst->pData[j] >= 'a' && pFirst->pData[j] <= 'z' )
					pFirst->pData[j] += 'A'-'a';
			}
		}

		/**
		 * Find the index of the first occurrance of (sText)
		 *@param sText (const char *) The string to search for.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long find( const char *sText )
		{
			long nTLen = strlen( sText );
			flatten();
			for( long j = 0; j < pFirst->nLength-nTLen; j++ )
			{
				if( !strncmp( sText, pFirst->pData+j, nTLen ) )
					return j;
			}
			return -1;
		}

		/**
		 * Do a reverse search for (sText)
		 *@param sText (const char *) The string to search for.
		 *@returns (long) The index of the last occurrance. -1 for not found.
		 */
		long rfind( const char *sText )
		{
			long nTLen = strlen( sText );
			flatten();
			for( long j = pFirst->nLength-nTLen-1; j >= 0; j-- )
			{
				if( !strncmp( sText, pFirst->pData+j, nTLen ) )
					return j;
			}
			return -1;
		}

		/**
		 * Function the archiver calls to archive your FString.
		 *@param ar (Archive) The archive which is archiving your FString.
		 */
		void archive( class Archive &ar )
		{
			if( ar.isLoading() )
			{
				clear();
				long nLen;
				ar >> nLen;

				if( nLen > 0 )
				{
					Chunk *pNew = newChunk( nLen );
					ar.read( pNew->pData, nLen*sizeof(chr) );
					appendChunk( pNew );
				}
			}
			else
			{
				flatten();
				
				ar << nLength;
				if( nLength )
					ar.write( pFirst->pData, nLength*sizeof(chr) );
			}
		}

	private:
		void flatten() const
		{
			if( isFlat() )
				return;

			if( pFirst == NULL )
				return;

			unShare();

			Chunk *pNew = newChunk( nLength );
			chr *pos = pNew->pData;
			Chunk *i = pFirst;
			for(;;)
			{
				cpy( pos, i->pData, i->nLength );
				pos += i->nLength;
				i = i->pNext;
				if( i == NULL )
					break;
			}
			realClear();

			pLast = pFirst = pNew;
			nLength = pNew->nLength;
		}
		
		void realClear() const
		{
			if( pFirst == NULL )
				return;

			if( isShared() )
			{
				decRefs();
			}
			else
			{
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
		}
		
		void copyFrom( const FBasicString<chr, nMinSize, chralloc, chunkalloc> &rSrc )
		{
			if( rSrc.pFirst == NULL )
				return;
			
			decRefs();

			Chunk *pNew = newChunk( rSrc.nLength );
			chr *pos = pNew->pData;
			Chunk *i = rSrc.pFirst;
			for(;;)
			{
				cpy( pos, i->pData, i->nLength );
				pos += i->nLength;
				i = i->pNext;
				if( i == NULL )
					break;
			}
			clear();

			appendChunk( pNew );
		}
		
		bool isFlat() const
		{
			return (pFirst == pLast);
		}

		bool isShared() const
		{
			return (pnRefs != NULL);
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
			pNew->pData = aChr.allocate( nLen+1 );
			pNew->pData[nLen] = (chr)0;
			return pNew;
		}

		void appendChunk( Chunk *pNewChunk )
		{
			unShare();

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
			unShare();

			if( pFirst == NULL )
				pLast = pFirst = pNewChunk;
			else
			{
				pNewChunk->pNext = pFirst;
				pFirst = pNewChunk;
			}

			nLength += pNewChunk->nLength;
		}

		void joinShare( MyType &rSrc )
		{
			clear();

			if( !rSrc.isFlat() )
				rSrc.flatten();

			rSrc.initCount();
			pnRefs = rSrc.pnRefs;
			(*pnRefs)++;
			nLength = rSrc.nLength;
			pFirst = rSrc.pFirst;
			pLast = rSrc.pLast;
		}
		
		void joinShare( const MyType &rSrc )
		{
			clear();

			rSrc.flatten();

			if( !rSrc.isShared() )
			{
				rSrc.pnRefs = new uint32_t;
				(*rSrc.pnRefs) = 1;
			}
			pnRefs = rSrc.pnRefs;
			(*pnRefs)++;
			nLength = rSrc.nLength;
			pFirst = rSrc.pFirst;
			pLast = rSrc.pLast;
		}

		/**
		 * This takes an object that was shared and makes a copy of the base data
		 * that was being shared so that this copy can be changed.  This should be
		 * added before any call that will change this object;
		 */
		void unShare() const
		{
			if( isShared() == false )
				return;
			if( pFirst == NULL )
				return;

			Chunk *pNew = newChunk( nLength );
			chr *pos = pNew->pData;
			Chunk *i = pFirst;
			for(;;)
			{
				cpy( pos, i->pData, i->nLength );
				pos += i->nLength;
				i = i->pNext;
				if( i == NULL )
					break;
			}
			decRefs();
			pLast = pFirst = pNew;
			nLength = pNew->nLength;
		}

		/**
		 * This decrements our ref count and pulls us out of the share.  If the ref
		 * count hits zero because of this, it destroys the share.  This is not
		 * safe to call on it's own, it's much better to call unShare.
		 */
		void decRefs() const
		{
			if( isShared() )
			{
				(*pnRefs)--;
				if( (*pnRefs) == 0 )
					destroyShare();
				else
				{
					pnRefs = NULL;
					pFirst = NULL;
					pLast = NULL;
					nLength = 0;
				}
			}
		}

		/**
		 * While the unShare function removes an instance from a share, this
		 * function destroys the data that was in the share, removing the share
		 * itself.  This should only be called when the refcount for the share has
		 * or is about to reach zero.
		 */
		void destroyShare() const
		{
			delete pnRefs;
			pnRefs = NULL;
			realClear();
		}

#ifdef VALTEST
		void cpy( chr *dest, const chr *src, long count ) const
		{
			for( int j = 0; j < count; j++ )
			{
				*dest = *src;
				dest++;
				src++;
			}
		}
#endif

		void initCount() const
		{
			if( !isShared() )
			{
				pnRefs = new uint32_t;
				(*pnRefs) = 1;
			}
		}

	private:
		mutable long nLength;
		mutable uint32_t *pnRefs;
		mutable Chunk *pFirst;
		mutable Chunk *pLast;

		mutable chralloc aChr;
		mutable chunkalloc aChunk;
	};

	typedef FBasicString<char> FString;

	template<> uint32_t __calcHashCode<FString>( const FString &k );
	template<> bool __cmpHashKeys<FString>( const FString &a, const FString &b );
}

#endif
