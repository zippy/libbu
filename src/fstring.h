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

	virtual ~FBasicString()
	{
	}

	void append( const chr *pData )
	{
		long nLen;
		for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
		
		Chunk *pNew = newChunk( nLen );
		memcpy( pNew->pData, pData, nLen * sizeof(chr) );

		appendChunk( pNew );
	}

	void append( const chr *pData, long nLen )
	{
		Chunk *pNew = newChunk( nLen );
		
		memcpy( pNew->pData, pData, nLen * sizeof(chr) );

		appendChunk( pNew );
	}

	void prepend( const chr *pData )
	{
		long nLen;
		for( nLen = 0; pData[nLen] != (chr)0; nLen++ );
		
		Chunk *pNew = newChunk( nLen );
		memcpy( pNew->pData, pData, nLen * sizeof(chr) );

		prependChunk( pNew );
	}

	void prepend( const chr *pData, long nLen )
	{
		Chunk *pNew = newChunk( nLen );
		
		memcpy( pNew->pData, pData, nLen * sizeof(chr) );

		prependChunk( pNew );
	}

	void clear()
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

	chr *c_str()
	{
		if( pFirst == NULL )
			return NULL;

		flatten();
		return pFirst->pData;
	}

	FBasicString<chr, chralloc, chunkalloc> &operator +=( const chr *pData )
	{
		append( pData );

		return (*this);
	}

	FBasicString<chr, chralloc, chunkalloc> &operator =( const chr *pData )
	{
		clear();
		append( pData );

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



private:
	void flatten()
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
			memcpy( pos, i->pData, i->nLength*sizeof(chr) );
			pos += i->nLength;
			i = i->pNext;
			if( i == NULL )
				break;
		}
		clear();

		appendChunk( pNew );
	}
	
	bool isFlat()
	{
		return (pFirst == pLast);
	}

	bool isShared()
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

private:
	long nLength;
	uint32_t *pnRefs;
	Chunk *pFirst;
	Chunk *pLast;

	chralloc aChr;
	chunkalloc aChunk;
};

typedef FBasicString<char> FString;

#endif
