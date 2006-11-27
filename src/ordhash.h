#ifndef ORD_HASH_H
#define ORD_HASH_H

#include "hash.h"
#include "tqsort.h"

template<typename key, typename value, typename cmpfnc, typename sizecalc = __calcNextTSize_fast, typename keyalloc = std::allocator<key>, typename valuealloc = std::allocator<value>, typename challoc = std::allocator<uint32_t> >
class OrdHash : public Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>
{
public:
	OrdHash() :
		bSorted( false ),
		aData( NULL )
	{
	}

	virtual ~OrdHash()
	{
	}

protected:
	virtual void invalidate()
	{
		bSorted = false;
		delete[] aData;
		aData = NULL;
	}

	virtual void onInsert()
	{
		invalidate();
	}

	virtual void onUpdate()
	{
		invalidate();
	}

	virtual void onDelete()
	{
		invalidate();
	}

	virtual void onReHash()
	{
		invalidate();
	}

	virtual std::pair<key,value> getAtPos( uint32_t nPos )
	{
		return Hash<key, value, sizecalc, keyalloc, valuealloc, challoc>::getAtPos( aData[nPos].nIndex );
	}

	virtual void buildIndex()
	{
		aData = new struct ind[this->nFilled];
		uint32_t k = 0;
		for( uint32_t j = 0; j < this->nCapacity; j++ )
		{
			if( this->isFilled( j ) )
			{
				if( !this->isDeleted( j ) )
				{
					aData[k].pVal = &(this->aValues[j]);
					aData[k].nIndex = j;
					k++;
				}
			}
		}

		tqsort<typename OrdHash<key, value, cmpfnc, sizecalc, keyalloc, valuealloc, challoc>::ind, cmpfnc, value **>( aData, this->nFilled );

		bSorted = true;
	}

	virtual uint32_t getFirstPos( bool &bFinished )
	{
		if( bSorted == false )
			buildIndex();

		return 0;
	}
	
	virtual uint32_t getNextPos( uint32_t nPos, bool &bFinished )
	{
		if( nPos+1 >= this->nFilled )
		{
			bFinished = true;
			return 0;
		}
		return ++nPos;
	}
public:
	typedef struct ind
	{
		value *pVal;
		uint32_t nIndex;
	} ind;
private:
	bool bSorted;
	ind *aData;
};

#endif
