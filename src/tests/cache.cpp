#include <stdio.h>

#include "bu/cache.h"
#include "bu/cachable.h"

class Bob : public Bu::Cachable
{
public:
	Bob()
	{
	}

	virtual ~Bob()
	{
	}

	long getCacheId() const
	{
		return 0;
	}

	int iInt;
};

int main()
{
	Bu::Cache<Bob> bobCache;

//	Bu::CPtr<Bob> pB = bobCache.insert( new Bob() );
}

