#ifndef F_STRING_H
#define F_STRING_H

#include <stdint.h>
#include <memory>

template< typename chr=char >
struct FStringChunk
{
	long nLength;
	chr *pData;
	FStringChunk *pNext;
};

/**
 *
 */
template< typename chr=char, typename chralloc=std::allocator<chr>, typename chunkalloc=std::allocator<struct FStringChunk<chr> > >
class FBasicString
{
private:
	typedef struct FStringChunk<chr> Chunk;
	typedef struct FBasicString<chr, chralloc, chunkalloc> MyType;

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

	FBasicString( MyType &rSrc ) :
		nLength( 0 ),
		pnRefs( NULL ),
		pFirst( NULL ),
		pLast( NULL )
	{
		joinShare( rSrc );
	}

	FBasicString( const FBasicString<chr, chralloc, chunkalloc> &rSrc ) :
		nLength( 0 ),
		pnRefs( NULL ),
		pFirst( NULL ),
		pLast( NULL )
	{
		// Here we have no choice but to copy, since the other guy is a const.
		// In the case that the source were flat, we could get a reference, it
		// would make some things faster, but not matter in many other cases.

		copyFrom( rSrc );
	}

	virtual ~FBasicString()
	{
		clear();
	}

	void append( const chr *pData )
	{
		long nLen;
		for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
		
		Chunk *pNew = newChunk( nLen );
		cpy( pNew->pData, pData, nLen );

		appendChunk( pNew );
	}

	void append( const chr *pData, long nLen )
	{
		Chunk *pNew = newChunk( nLen );
		
		cpy( pNew->pData, pData, nLen );

		appendChunk( pNew );
	}

	void prepend( const chr *pData )
	{
		long nLen;
		for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
		
		Chunk *pNew = newChunk( nLen );
		cpy( pNew->pData, pData, nLen );

		prependChunk( pNew );
	}

	void prepend( const chr *pData, long nLen )
	{
		Chunk *pNew = newChunk( nLen );
		
		cpy( pNew->pData, pData, nLen );

		prependChunk( pNew );
	}

	void clear()
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

	chr *c_str()
	{
		if( pFirst == NULL )
			return NULL;

		flatten();
		return pFirst->pData;
	}

	MyType &operator +=( const chr *pData )
	{
		append( pData );

		return (*this);
	}

	MyType &operator =( const chr *pData )
	{
		clear();
		append( pData );

		return (*this);
	}

	MyType &operator =( const MyType &rSrc )
	{
		if( rSrc.isFlat() )
		{
			joinShare( rSrc );
		}
		else
		{
			copyFrom( rSrc );
		}

		return (*this);
	}
	
	MyType &operator =( MyType &rSrc )
	{
		joinShare( rSrc );

		return (*this);
	}

	bool operator ==( const chr *pData )
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

	bool operator !=(const chr *pData )
	{
		return !(*this == pData);
	}

	chr &operator[]( long nIndex )
	{
		flatten();

		return pFirst->pData[nIndex];
	}

private:
	void flatten()
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
		clear();

		appendChunk( pNew );
	}
	
	void copyFrom( const FBasicString<chr, chralloc, chunkalloc> &rSrc )
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

	Chunk *newChunk()
	{
		Chunk *pNew = aChunk.allocate( 1 );
		pNew->pNext = NULL;
		return pNew;
	}
	
	Chunk *newChunk( long nLen )
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

		if( !rSrc.isFlat() )
			return;

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
	void unShare()
	{
		if( isShared() == false )
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
		appendChunk( pNew );
		decRefs();
	}

	/**
	 * This decrements our ref count and pulls us out of the share.  If the ref
	 * count hits zero because of this, it destroys the share.  This is not
	 * safe to call on it's own, it's much better to call unShare.
	 */
	void decRefs()
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
	void destroyShare()
	{
		delete pnRefs;
		pnRefs = NULL;
		clear();
	}

	void cpy( chr *dest, const chr *src, long count )
	{
		for( int j = 0; j < count; j++ )
		{
			*dest = *src;
			dest++;
			src++;
		}
	}

	void initCount() const
	{
		if( !isShared() )
		{
			pnRefs = new uint32_t;
			(*pnRefs) = 1;
		}
	}

private:
	long nLength;
	mutable uint32_t *pnRefs;
	Chunk *pFirst;
	Chunk *pLast;

	chralloc aChr;
	chunkalloc aChunk;
};

typedef FBasicString<char> FString;

#endif
