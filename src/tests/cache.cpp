#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "bu/cache.h"
#include "bu/file.h"
#include "bu/fstring.h"

class Bob
{
public:
	Bob()
	{
		TRACE();
	}

	Bob( int i ) :
		iInt( i )
	{
		TRACE( i );
	}

	virtual ~Bob()
	{
		TRACE();
	}

	void setInt( int i )
	{
		TRACE( i );
		iInt = i;
	}

	int getInt()
	{
		return iInt;
	}

	int iInt;
};

namespace Bu {
	template<>
	void __tracer_format<Bob*>( Bob* const &c )
	{
		printf("%08X=%d", (uint32_t)c, c->getInt() );
	}
}

class BobStore : public Bu::CacheStore<Bob, long>
{
public:
	BobStore() :
		cLastId( 0 )
	{
		TRACE();
		if( access( "bobcache/last", R_OK|W_OK ) )
		{
			mkdir("bobcache", 0755 );
			Bu::File f("bobcache/last", Bu::File::Write|Bu::File::Create );
			f.write("0", 1);
			printf("Initializing cache:  %s\n", strerror( errno ) );
		}
		else
		{
			cLastId = readNum("bobcache/last");
		}
	}

	~BobStore()
	{
		TRACE();
		writeNum("bobcache/last", cLastId );
	}

	long readNum( const Bu::FString &sFile )
	{
		TRACE( sFile );
		Bu::File f( sFile, Bu::File::Read );
		char buf[80];
		buf[f.read( buf, 80 )] = '\0';
		return strtol( buf, NULL, 0 );
	}

	void writeNum( const Bu::FString &sFile, long num )
	{
		TRACE( sFile, num );
		Bu::File f( sFile,
			Bu::File::Write|Bu::File::Create|Bu::File::Truncate
			);
		Bu::FString s;
		s.format("%d", num );
		f.write( s );
	}

	virtual Bob *load( const long &key )
	{
		TRACE( key );
		Bu::FString sDest;
		sDest.format("bobcache/%d", key );
		return  new Bob( readNum( sDest ) );
	}

	virtual void unload( Bob *pObj, const long &key )
	{
		TRACE( pObj, key );
		Bu::FString sDest;
		sDest.format("bobcache/%d", key );
		writeNum( sDest, pObj->getInt() );
		delete pObj;
	}

	virtual long create( Bob *rSrc )
	{
		TRACE( rSrc );
		long id = ++cLastId;
		Bu::FString sDest;
		sDest.format("bobcache/%d", id );
		writeNum( sDest, rSrc->getInt() );
		return id;
	}

	virtual void destroy( Bob *pObj, const long &key )
	{
		TRACE( pObj, key );
		Bu::FString sDest;
		sDest.format("bobcache/%d", key );
		if( !access( sDest.getStr(), F_OK ) )
			unlink( sDest.getStr() );
		delete pObj;
	}

private:
	long cLastId;
};

int main( int argc, char *argv[] )
{
	TRACE( argc, argv );
	typedef Bu::Cache<Bob, long> BobCache;
	typedef BobCache::Ptr BobPtr;

	if( argc < 3 )
	{
		printf("Try: %s [crud] [<id/value>]\n\n", argv[0] );
		return 0;
	}

	BobCache cBob;
	cBob.appendStore( new BobStore() );
	switch( argv[1][0] )
	{
		case 'c':
			{
				BobCache::Ptr pBob = cBob.insert(
					new Bob( strtol( argv[2], NULL, 0 ) )
					);
				printf("Key = %ld\n", pBob.getKey() );
			}
			return 0;

		case 'r':
			{
				BobCache::Ptr pBob = cBob.get( strtol( argv[2], NULL, 0 ) );
				printf("Value = %d\n", pBob->getInt() );
			}
			return 0;

		case 'u':
			{
				BobCache::Ptr pBob = cBob.get( strtol( argv[2], NULL, 0 ) );
				pBob->setInt( strtol( argv[3], NULL, 0 ) );
			}
			return 0;

		case 'd':
			{
				cBob.erase( strtol( argv[2], NULL, 0 ) );
			}
			return 0;
	}

}

