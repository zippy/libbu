/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#include "bu/cache.h"
#include "bu/file.h"
#include "bu/fstring.h"
#include "bu/cachecalc.h"

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
	
	long getKey() const
	{
		TRACE( i );
		return iInt;
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

class BobStore : public Bu::CacheStore<long, Bob>
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

	virtual void sync( Bob *, const long & )
	{
	}

	virtual void sync()
	{
	}

	virtual bool has( const long & )
	{
		return true;
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

	virtual void destroy( const long &key )
	{
		TRACE( pObj, key );
		Bu::FString sDest;
		sDest.format("bobcache/%d", key );
		if( !access( sDest.getStr(), F_OK ) )
			unlink( sDest.getStr() );
	}

private:
	long cLastId;
};

class BobCalc : public Bu::CacheCalc<long, Bob>
{
public:
	BobCalc()
	{
	}

	virtual ~BobCalc()
	{
	}

	virtual void onLoad( Bob *, const long & )
	{
	}

	virtual void onUnload( Bob *, const long & )
	{
	}

	virtual void onDestroy( Bob *, const long & )
	{
	}

	virtual void onDestroy( const long & )
	{
	}

	virtual bool shouldSync( Bob *, const long &, time_t )
	{
		return false;
	}

private:

};

int main( int argc, char *argv[] )
{
	TRACE( argc, argv );
	typedef Bu::Cache<long, Bob> BobCache;
	typedef BobCache::Ptr BobPtr;

	if( argc < 3 )
	{
		printf("Try: %s [crudl] [<id/value>]\n\n", argv[0] );
		return 0;
	}

	BobCache cBob( new BobCalc(), new BobStore() );
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

		case 'l':
			{
				BobCache::Ptr pBob = cBob.getLazy( strtol( argv[2], NULL, 0 ) );
				printf("isBound:  %s\n", pBob.isBound()?"yes":"no");
				printf("Value = %d\n", pBob->getInt() );
				printf("isBound:  %s\n", pBob.isBound()?"yes":"no");
			}
			return 0;
	}

}

