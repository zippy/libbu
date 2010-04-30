/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CACHE_STORE_NIDS_H
#define BU_CACHE_STORE_NIDS_H

#include "bu/fstring.h"
#include "bu/stream.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"
#include "bu/cachestore.h"

#include "bu/file.h"
#include "bu/archive.h"

namespace Bu
{
	template<class keytype, class obtype>
	keytype __cacheGetKey( const obtype *pObj );

	template<class keytype, class obtype>
	obtype *__cacheStoreNidsAlloc( const keytype &key )
	{
		return new obtype();
	}

	template<class keytype, class obtype>
	void __cacheStoreNidsStore( Bu::Stream &s, obtype &rObj,
			const keytype & )
	{
		Bu::Archive ar( s, Bu::Archive::save );
		ar << rObj;
	}

	template<class keytype, class obtype>
	obtype *__cacheStoreNidsLoad( Bu::Stream &s, const keytype &key )
	{
		obtype *pObj = __cacheStoreNidsAlloc<keytype, obtype>( key );
		Bu::Archive ar( s, Bu::Archive::load );
		ar >> (*pObj);
		return pObj;
	}

	template<class keytype, class obtype>
	class CacheStoreNids : public CacheStore<keytype, obtype>
	{
	public:
		CacheStoreNids( Bu::Stream &sArch,
			int iBlockSize=1024, int iPreAllocate=1 ) :
			nStore( sArch )
		{
			try
			{
				nStore.initialize();
				NidsStream ns = nStore.openStream( 0 );
				Bu::Archive ar( ns, Bu::Archive::load );
				ar >> hId;
			}
			catch( Bu::NidsException &e )
			{
				nStore.initialize( iBlockSize, iPreAllocate );
				int iStream = nStore.createStream();
				if( iStream != 0 )
					throw Bu::ExceptionBase("That's...horrible...id = %d.\n\n", iStream );
				NidsStream ns = nStore.openStream( 0 );
				Bu::Archive ar( ns, Bu::Archive::save );
				ar << hId;
			}
		}

		virtual ~CacheStoreNids()
		{
			NidsStream ns = nStore.openStream( 0 );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << hId;
		}
		
		virtual obtype *load( const keytype &key )
		{
			int iStream = hId.get( key );
			NidsStream ns = nStore.openStream( iStream );
			obtype *pOb = __cacheStoreNidsLoad<keytype, obtype>( ns, key );
			return pOb;
		}

		virtual void unload( obtype *pObj, const keytype &key )
		{
			delete pObj;
		}

		virtual keytype create( obtype *pSrc )
		{
			keytype key = __cacheGetKey<keytype, obtype>( pSrc );
			int iStream = nStore.createStream();
			hId.insert( key, iStream );
			NidsStream ns = nStore.openStream( iStream );
			__cacheStoreNidsStore<keytype, obtype>( ns, *pSrc, key );
			return key;
		}

		virtual void sync()
		{
			NidsStream ns = nStore.openStream( 0 );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << hId;
		}

		virtual void sync( obtype *pSrc, const keytype &key )
		{
			int iStream = hId.get( key );
			NidsStream ns = nStore.openStream( iStream );
			__cacheStoreNidsStore<keytype, obtype>( ns, *pSrc, key );
		}

		virtual void destroy( obtype *pObj, const keytype &key )
		{
			int iStream = hId.get( key );
			nStore.deleteStream( iStream );
			hId.erase( key );
			delete pObj;
		}

		virtual bool has( const keytype &key )
		{
			return hId.has( key );
		}

		virtual Bu::List<keytype> getKeys()
		{
			return hId.getKeys();
		}

		virtual int getSize()
		{
			return hId.getSize();
		}

	private:
		Nids nStore;
		typedef Bu::Hash<keytype, long> NidHash;
		NidHash hId;
	};
};

#endif
