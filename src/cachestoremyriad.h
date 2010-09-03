/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CACHE_STORE_MYRIAD_H
#define BU_CACHE_STORE_MYRIAD_H

#include "bu/fstring.h"
#include "bu/stream.h"
#include "bu/myriad.h"
#include "bu/cachestore.h"
#include "bu/myriadstream.h"

#include "bu/archive.h"

namespace Bu
{
	template<class keytype, class obtype>
	keytype __cacheGetKey( const obtype *pObj );

	template<class keytype, class obtype>
	obtype *__cacheStoreMyriadAlloc( const keytype &key )
	{
		return new obtype();
	}

	template<class keytype, class obtype>
	void __cacheStoreMyriadStore( Bu::Stream &s, obtype &rObj,
			const keytype & )
	{
		Bu::Archive ar( s, Bu::Archive::save );
		ar << rObj;
	}

	template<class keytype, class obtype>
	obtype *__cacheStoreMyriadLoad( Bu::Stream &s, const keytype &key )
	{
		obtype *pObj = __cacheStoreMyriadAlloc<keytype, obtype>( key );
		Bu::Archive ar( s, Bu::Archive::load );
		ar >> (*pObj);
		return pObj;
	}

	template<class keytype, class obtype>
	class CacheStoreMyriad : public CacheStore<keytype, obtype>
	{
	public:
		CacheStoreMyriad( Bu::Stream &sArch,
			int iBlockSize=512, int iPreAllocate=8 ) :
			mStore( sArch, iBlockSize, iPreAllocate )
		{
			try
			{
				MyriadStream ns = mStore.openStream( 1 );
				Bu::Archive ar( ns, Bu::Archive::load );
				ar >> hId;
			}
			catch( Bu::MyriadException &e )
			{
				int iStream = mStore.createStream();
				if( iStream != 1 )
					throw Bu::ExceptionBase("That's...horrible...id = %d.\n\n",
							iStream );
				MyriadStream ns = mStore.openStream( 1 );
				Bu::Archive ar( ns, Bu::Archive::save );
				ar << hId;
			}
		}

		virtual ~CacheStoreMyriad()
		{
			MyriadStream ns = mStore.openStream( 1 );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << hId;
		}
		
		virtual obtype *load( const keytype &key )
		{
			int iStream = hId.get( key );
			MyriadStream ns = mStore.openStream( iStream );
			obtype *pOb = __cacheStoreMyriadLoad<keytype, obtype>( ns, key );
			return pOb;
		}

		virtual void unload( obtype *pObj, const keytype &key )
		{
			delete pObj;
		}

		virtual keytype create( obtype *pSrc )
		{
			keytype key = __cacheGetKey<keytype, obtype>( pSrc );
			int iStream = mStore.createStream();
			hId.insert( key, iStream );
			MyriadStream ns = mStore.openStream( iStream );
			__cacheStoreMyriadStore<keytype, obtype>( ns, *pSrc, key );
			ns.setSize( ns.tell() );
			return key;
		}

		virtual void sync()
		{
			MyriadStream ns = mStore.openStream( 1 );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << hId;
			ns.setSize( ns.tell() );
			mStore.sync();
		}

		virtual void sync( obtype *pSrc, const keytype &key )
		{
			int iStream = hId.get( key );
			MyriadStream ns = mStore.openStream( iStream );
			__cacheStoreMyriadStore<keytype, obtype>( ns, *pSrc, key );
			ns.setSize( ns.tell() );
		}

		virtual void destroy( obtype *pObj, const keytype &key )
		{
			int iStream = hId.get( key );
			mStore.deleteStream( iStream );
			hId.erase( key );
			delete pObj;
		}

		virtual void destroy( const keytype &key )
		{
			int iStream = hId.get( key );
			mStore.deleteStream( iStream );
			hId.erase( key );
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
		Myriad mStore;
		typedef Bu::Hash<keytype, long> StreamHash;
		StreamHash hId;
	};
};

#endif
