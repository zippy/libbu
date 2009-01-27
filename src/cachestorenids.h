#ifndef BU_CACHE_STORE_NIDS_H
#define BU_CACHE_STORE_NIDS_H

#include "bu/fstring.h"
#include "bu/stream.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"
#include "bu/cachestore.h"

#include "bu/file.h"

namespace Bu
{
	template<class obtype, class keytype>
	keytype __cacheGetKey( const obtype *pObj );

	template<class obtype, class keytype>
	obtype *__cacheStoreNidsAlloc( const keytype &key )
	{
		return new obtype();
	}

	template<class obtype, class keytype>
	class CacheStoreNids : public CacheStore<obtype, keytype>
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
					printf("That's...horrible...id = %d.\n\n", iStream );
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
			Bu::Archive ar( ns, Bu::Archive::load );
			obtype *pOb = __cacheStoreNidsAlloc<obtype, keytype>( key );
			ar >> (*pOb);
			return pOb;
		}

		virtual void unload( obtype *pObj, const keytype &key )
		{
			int iStream = hId.get( key );
			NidsStream ns = nStore.openStream( iStream );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << (*pObj);
			delete pObj;
		}

		virtual keytype create( obtype *pSrc )
		{
			keytype key = __cacheGetKey<obtype,keytype>( pSrc );
			int iStream = nStore.createStream();
			hId.insert( key, iStream );
			NidsStream ns = nStore.openStream( iStream );
			Bu::Archive ar( ns, Bu::Archive::save );
			ar << (*pSrc);
			return key;
		}

		virtual void destroy( obtype *pObj, const keytype &key )
		{
			int iStream = hId.get( key );
			nStore.deleteStream( iStream );
			hId.erase( key );
			delete pObj;
		}

		virtual Bu::List<keytype> getKeys()
		{
			return hId.getKeys();
		}

	private:
		Nids nStore;
		typedef Bu::Hash<keytype, long> NidHash;
		NidHash hId;
	};
};

#endif
