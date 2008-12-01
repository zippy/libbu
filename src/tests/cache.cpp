#include <stdio.h>

#include "bu/cache.h"

class Bob
{
public:
	Bob()
	{
		TRACE();
	}

	virtual ~Bob()
	{
		TRACE();
	}

	void setInt( int i )
	{
		TRACE();
		iInt = i;
	}

	int getInt()
	{
		return iInt;
	}

	long getCacheId() const
	{
		TRACE();
		return 0;
	}

	int iInt;
};

class BobHandler : public Bu::CacheHandler<Bob>
{
public:
	BobHandler() :
		cLastId( 0 )
	{
	}

	~BobHandler()
	{
	}

	virtual Bu::CPtr<Bob> load()
	{
	}

	virtual void unload( Bu::CPtr<Bob> pObj )
	{
	}

	virtual Bu::CPtr<Bob> create()
	{
	}

	virtual Bu::CPtr<Bob> create( Bob &rSrc )
	{
	}

	virtual void destroy( Bu::CPtr<Bob> pObj )
	{
	}

private:
	Bu::Cache<Bob>::cid_t cLastId;
};

int main()
{
	typedef Bu::Cache<Bob> BobCache;
	typedef Bu::CPtr<Bob> BobPtr;

	BobCache bobCache;

	BobPtr pB1 = bobCache.insert( new Bob() );
	(*pB1).setInt( 44 );
	printf("RefCnt = %d\n", bobCache.getRefCnt( 0 ) );

	BobPtr pB2 = bobCache.get( 0 );
	printf("RefCnt = %d\n", bobCache.getRefCnt( 0 ) );
	printf("Int = %d\n", pB2->getInt() );
}

