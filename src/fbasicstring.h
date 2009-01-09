/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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

#include "bu/archival.h"
#include "bu/archive.h"
#include "bu/util.h"

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
	class FBasicString : public Archival
	{
#ifndef VALTEST
#define cpy( dest, src, size ) memcpy( dest, src, size*sizeof(chr) )
#endif
	private:
		//template< typename chr >
	/*	struct Chunk
		{
			long nLength;
			chr *pData;
			FChunk *pNext;
		}; */

		typedef struct FStringChunk<chr> Chunk;
		typedef struct FBasicString<chr, nMinSize, chralloc, chunkalloc> MyType;

	public:
		FBasicString() :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
		}

		FBasicString( const chr *pData ) :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( pData );
		}

		FBasicString( const chr *pData, long nLength ) :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( pData, nLength );
		}

		FBasicString( const MyType &rSrc ) :
			Archival(),
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
			if( rSrc.nLength > 0 )
			{
				rSrc.flatten();
				append( rSrc.pFirst->pData, rSrc.nLength );
			}
		}

		FBasicString( const MyType &rSrc, long nLength ) :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( rSrc.pFirst->pData, nLength );
		}
		
		FBasicString( const MyType &rSrc, long nStart, long nLength ) :
			nLength( 0 ),
			pFirst( NULL ),
			pLast( NULL )
		{
			append( rSrc.pFirst->pData+nStart, nLength );
		}

		FBasicString( long nSize ) :
			nLength( nSize ),
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
			if( !pData ) return;
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ ) { }
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
			if( pLast && pLast->nLength < nMinSize )
			{
				pLast->pData[pLast->nLength] = cData;
				++pLast->nLength; ++nLength;
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
		 */
		void append( const MyType & sData )
		{
			append( sData.getStr(), sData.getSize() );
		}
		
		/**
		 * Append another FString to this one.
		 *@param sData (MyType &) The FString to append.
		 *@param nLen How much data to append.
		 */
		void append( const MyType & sData, long nLen )
		{
			append( sData.getStr(), nLen );
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
			if( pData == NULL )
				return;
			long nLen;
			for( nLen = 0; pData[nLen] != (chr)0; nLen++ ) { }
			
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

		void insert( long nPos, const chr *pData, long nLen )
		{
			if( nLen <= 0 )
				return;
			if( nPos <= 0 )
			{
				prepend( pData, nLen );
			}
			else if( nPos >= nLength )
			{
				append( pData, nLen );
			}
			else
			{
				flatten();
				Chunk *p1 = newChunk( nPos );
				Chunk *p2 = newChunk( nLen );
				Chunk *p3 = newChunk( nLength-nPos );
				cpy( p1->pData, pFirst->pData, nPos );
				cpy( p2->pData, pData, nLen );
				cpy( p3->pData, pFirst->pData+nPos, nLength-nPos );
				clear();
				appendChunk( p1 );
				appendChunk( p2 );
				appendChunk( p3 );
			}
		}

		void insert( long nPos, const MyType &str )
		{
			if( nPos <= 0 )
			{
				prepend( str );
			}
			else if( nPos >= nLength )
			{
				append( str );
			}
			else
			{
				flatten();
				Chunk *p1 = newChunk( nPos );
				Chunk *p3 = newChunk( nLength-nPos );
				cpy( p1->pData, pFirst->pData, nPos );
				cpy( p3->pData, pFirst->pData+nPos, nLength-nPos );
				clear();
				appendChunk( p1 );
				for( Chunk *pChnk = str.pFirst; pChnk; pChnk = pChnk->pNext )
				{
					appendChunk( copyChunk( pChnk ) );
				}

				appendChunk( p3 );
			}
		}

		void insert( long nPos, const chr *pData )
		{
			insert( nPos, pData, strlen( pData ) );
		}

		void remove( long nPos, long nLen )
		{
			if( nLen <= 0 || nPos < 0 || nPos >= nLength )
				return;
			if( nLen > nLength-nPos )
				nLen = nLength-nPos;
			flatten();
			cpy( pFirst->pData+nPos, pFirst->pData+nPos+nLen, nLength-nPos-nLen+1 );
			nLength -= nLen;
			pFirst->nLength -= nLen;
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
			if( nNewSize < 0 )
				nNewSize = 0;

			flatten();

			Chunk *pNew = newChunk( nNewSize );
			long nNewLen = (nNewSize<nLength)?(nNewSize):(nLength);
			if( nLength > 0 )
			{
				cpy( pNew->pData, pFirst->pData, nNewLen );
				aChr.deallocate( pFirst->pData, pFirst->nLength+1 );
				aChunk.deallocate( pFirst, 1 );
			}
			pNew->pData[nNewLen] = (chr)0;
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
				return (chr *)"";

			flatten();
			pFirst->pData[nLength] = (chr)0;
			return pFirst->pData;
		}
		
		/**
		 * Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		const chr *getStr() const
		{
			if( pFirst == NULL )
				return (chr *)"";

			flatten();
			pFirst->pData[nLength] = (chr)0;
			return pFirst->pData;
		}

		MyType getSubStr( long iStart, long iSize=-1 ) const
		{
			if( iStart < 0 )
				iStart = 0;
			if( iStart >= nLength )
				return "";
			if( iSize < 0 )
				iSize = nLength;
			if( iStart+iSize > nLength )
				iSize = nLength-iStart;
			if( iSize == 0 )
				return "";

			flatten();
			MyType ret( pFirst->pData+iStart, iSize );
			return ret;
		}

		/**
		 * (std::string compatability) Get a pointer to the string array.
		 *@returns (chr *) The string data.
		 */
		DEPRECATED
		chr *c_str()
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			pFirst->pData[nLength] = (chr)0;
			return pFirst->pData;
		}
		
		/**
		 * (std::string compatability) Get a const pointer to the string array.
		 *@returns (const chr *) The string data.
		 */
		DEPRECATED
		const chr *c_str() const
		{
			if( pFirst == NULL )
				return NULL;

			flatten();
			pFirst->pData[nLength] = (chr)0;
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
		MyType &operator +=( const chr cData )
		{
			if( pLast && pLast->nLength < nMinSize )
			{
				pLast->pData[pLast->nLength] = cData;
				++pLast->nLength; ++nLength;
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
		MyType &operator =( const chr *pData )
		{
			clear();
			append( pData );

			return (*this);
		}

		MyType &operator =( const std::basic_string<chr> &rData )
		{
			clear();
			append( rData.c_str(), rData.size() );

			return (*this);
		}

		MyType operator +( const MyType &rRight )
		{
			MyType ret( *this );
			ret.append( rRight );
			return ret;
		}

		MyType operator +( const chr *pRight )
		{
			MyType ret( *this );
			ret.append( pRight );
			return ret;
		}

		MyType operator +( chr *pRight )
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

		void expand()
		{
			flatten();

#ifndef WIN32
			wordexp_t result;

			/* Expand the string for the program to run.  */
			switch (wordexp (pFirst->pData, &result, 0))
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
		MyType &operator =( const MyType &rSrc )
		{
			copyFrom( rSrc );

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
				if( pData[0] == (chr)0 )
					return true;
				return false;
			}

			flatten();
			pFirst->pData[nLength] = (chr)0;
			const chr *a = pData;
			chr *b = pFirst->pData;
			for( long j = 0; *a!=(chr)0 || *b!=(chr)0; j++, a++, b++ )
			{
				if( *a != *b )
					return false;
				if( *a == (chr)0 && j < nLength )
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
			if( nLength != pData.nLength )
				return false;

			flatten();
			pData.flatten();
			const chr *a = pData.pFirst->pData;
			chr *b = pFirst->pData;
			for( long j = 0; j < nLength; j++, a++, b++ )
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
			return (pFirst != NULL);
		}

		bool isSet() const
		{
			return (pFirst != NULL);
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

			for( long j = 0; j < nLength; j++ )
			{
				if( pFirst->pData[j] >= 'a' && pFirst->pData[j] <= 'z' )
					pFirst->pData[j] += 'A'-'a';
			}
		}

		/**
		 * Find the index of the first occurrance of (sText)
		 *@param sText (const chr *) The string to search for.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long find( const chr cChar ) const
		{
			flatten();
			for( long j = 0; j < pFirst->nLength; j++ )
			{
				if( pFirst->pData[j] == cChar )
					return j;
			}
			return -1;
		}
		
		/**
		 * Find the index of the first occurrance of cChar
		 *@param cChar (const chr) The character to search for.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long find( const chr *sText ) const
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
		 * Find the index of the first occurrance of cChar
		 *@param sText (const chr *) The string to search for.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long find( long iStart, const chr cChar ) const
		{
			flatten();
			for( long j = iStart; j < pFirst->nLength; j++ )
			{
				if( pFirst->pData[j] == cChar )
					return j;
			}
			return -1;
		}
		
		/**
		 * Find the index of the first occurrance of sText
		 *@param cChar (const chr) The character to search for.
		 *@returns (long) The index of the first occurrance. -1 for not found.
		 */
		long find( long iStart, const chr *sText ) const
		{
			long nTLen = strlen( sText );
			flatten();
			for( long j = iStart; j < pFirst->nLength-nTLen; j++ )
			{
				if( !strncmp( sText, pFirst->pData+j, nTLen ) )
					return j;
			}
			return -1;
		}

		/**
		 * Do a reverse search for (sText)
		 *@param sText (const chr *) The string to search for.
		 *@returns (long) The index of the last occurrance. -1 for not found.
		 */
		long rfind( const chr *sText ) const
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
		 * Remove nAmnt bytes from the front of the string.  This function
		 * operates in O(n) time and should be used sparingly.
		 */
		void trimFront( long nAmnt )
		{
			long nNewLen = nLength - nAmnt;
			flatten();
			Chunk *pNew = newChunk( nNewLen );
			cpy( pNew->pData, pFirst->pData+nAmnt, nNewLen );
			clear();
			appendChunk( pNew );
		}

		void format( const char *sFrmt, ...)
		{
			clear();

			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );
			
			Chunk *pNew = newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			appendChunk( pNew );

			va_end( ap );
		}

		void formatAppend( const char *sFrmt, ...)
		{
			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );
			
			Chunk *pNew = newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			appendChunk( pNew );

			va_end( ap );
		}

		void formatPrepend( const char *sFrmt, ...)
		{
			va_list ap;
			va_start( ap, sFrmt );

			long iLen = vsnprintf( NULL, 0, sFrmt, ap );
			
			Chunk *pNew = newChunk( iLen );
			vsnprintf( pNew->pData, iLen+1, sFrmt, ap );
			prependChunk( pNew );

			va_end( ap );
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

		typedef chr *iterator;
		typedef const chr *const_iterator;

		iterator begin()
		{
			if( nLength == 0 )
				return NULL;
			flatten();
			return pFirst->pData;
		}

		const_iterator begin() const
		{
			if( nLength == 0 )
				return NULL;
			flatten();
			return pFirst->pData;
		}

		iterator end()
		{
			if( nLength == 0 )
				return NULL;
			return pFirst->pData+pFirst->nLength;
		}

		const_iterator end() const
		{
			if( nLength == 0 )
				return NULL;
			return pFirst->pData+pFirst->nLength;
		}

		bool isEmpty() const
		{
			if( nLength == 0 )
				return true;
			return false;
		}

	private:
		void flatten() const
		{
			if( isFlat() )
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
			realClear();

			pLast = pFirst = pNew;
			nLength = pNew->nLength;
		}
		
		void realClear() const
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
		
		void copyFrom( const FBasicString<chr, nMinSize, chralloc, chunkalloc> &rSrc )
		{
			if( rSrc.pFirst == NULL )
			{
				clear();
				return;
			}
			
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

	private:
		mutable long nLength;
		mutable Chunk *pFirst;
		mutable Chunk *pLast;

		mutable chralloc aChr;
		mutable chunkalloc aChunk;
	};
	
	template<class T> FBasicString<T> operator +( const T *pLeft, const FBasicString<T> &rRight )
	{
		Bu::FBasicString<T> ret( pLeft );
		ret.append( rRight );
		return ret;
	}
}

#ifndef VALTEST
#undef cpy
#endif

#endif