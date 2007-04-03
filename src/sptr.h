#ifndef SPTR_H
#define SPTR_H

#include <stdint.h>
#include <stdio.h>

template<typename T>
class SPtr
{
public:
	SPtr() :
		pRefCnt( NULL ),
		pData( NULL )
	{
	}

	~SPtr()
	{
		decCount();
	}

	SPtr( const SPtr<T> &src ) :
		pRefCnt( src.pRefCnt ),
		pData( src.pData )
	{
		if( pRefCnt )
			(*pRefCnt) += 1;
	}

	SPtr( T *pSrc ) :
		pRefCnt( NULL ),
		pData( pSrc )
	{
		pRefCnt = new int32_t;
		(*pRefCnt) = 1;
	}

	int32_t count()
	{
		return *pRefCnt;
	}

	T *operator->() const
	{
		return pData;
	}

	T *operator*() const
	{
		return pData;
	}

	SPtr<T> operator=( const SPtr<T> &src )
	{
		decCount();
		pRefCnt = src.pRefCnt;
		pData = src.pData;
		(*pRefCnt) += 1;

		return *this;
	}

	bool operator==( const SPtr<T> &src )
	{
		return pData == src.pData;
	}

	operator bool() const
	{
		return pRefCnt != NULL;
	}

	bool isSet() const
	{
		return pRefCnt != NULL;
	}

private:
	void decCount()
	{
		if( pRefCnt )
		{
			(*pRefCnt) -= 1;
			//printf("Decrementing ref-count to %d\n", *pRefCnt );
			if( (*pRefCnt) == 0 )
			{
				delete pRefCnt;
				delete pData;
				pRefCnt = NULL;
				pData = NULL;
			}
		}
	}

	int32_t *pRefCnt;
	T *pData;
};

#endif
