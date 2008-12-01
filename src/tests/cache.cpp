#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

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

class BobStore : public Bu::CacheStore<Bob, long>
{
public:
	BobStore() :
		cLastId( 0 )
	{
		TRACE();
	}

	~BobStore()
	{
		TRACE();
	}

	virtual Bob *load( const long &key )
	{
		TRACE();
		return NULL;
	}

	virtual void unload( Bob *pObj )
	{
		TRACE();
		delete pObj;
	}

	virtual long create( Bob *rSrc )
	{
		TRACE();
		return ++cLastId;
	}

	virtual void destroy( Bob *pObj, const long &key )
	{
		TRACE();
		delete pObj;
	}

private:
	long cLastId;
};

int main( int argc, char *argv[] )
{
	TRACE();
	if( argc < 3 )
	{
		printf("Try: %s [icufd] [<id/value>]\n\n", argv[0] );
		return 0;
	}

	switch( argv[1][0] )
	{
		case 'i':
			mkdir("bobcache", 0755 );
			printf("Initialized cache:  %s\n", strerror( errno ) );
			return 0;

		case 'c':
			typedef Bu::Cache<Bob, long> BobCache;
			typedef BobCache::Ptr BobPtr;

			BobCache cBob;

			cBob.appendStore( new BobStore() );

			return 0;
	}

}

