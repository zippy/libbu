#include <stdio.h>

#include "bu/cache.h"
#include "bu/cachable.h"

class Bob : public Bu::Cachable
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

DECL_CACHABLE( Bob );
DEF_CACHABLE( Bob );

int main()
{
	Bu::Cache<Bob> bobCache;

	Bu::CPtr<Bob> pB1 = bobCache.insert( new Bob() );

	(*pB1).setInt( 44 );
	
	printf("RefCnt = %d\n", bobCache.getRefCnt( 0 ) );

	Bu::CPtr<Bob> pB2 = bobCache.get( 0 );
	
	printf("RefCnt = %d\n", bobCache.getRefCnt( 0 ) );

	printf("Int = %d\n", pB2->getInt() );

}

