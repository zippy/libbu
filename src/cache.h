#ifndef BU_CACHE_H
#define BU_CACHE_H

#include "bu/cptr.h"
#include "bu/hash.h"
#include "bu/list.h"
#include "bu/cachestore.h"

#define BU_TRACE
#include "bu/trace.h"

namespace Bu
{
	template<class obtype, class keytype>
	class Cache
	{
	friend class Bu::CPtr<obtype, keytype>;
	public:
		typedef Bu::CPtr<obtype, keytype> Ptr;
	private:
		typedef Bu::CacheStore<obtype, keytype> Store;
		typedef Bu::List<Store *> StoreList;
		
		typedef struct CacheEntry
		{
			obtype *pData;
			int iRefs;
		} CacheEntry;

		typedef Bu::Hash<keytype, CacheEntry> CidHash;

	public:
		Cache()
		{
			TRACE();
		}

		virtual ~Cache()
		{
			TRACE();
			for( typename CidHash::iterator i = hEnt.begin();
				 i != hEnt.end(); i++ )
			{
				if( i.getValue().iRefs > 0 )
				{
					printf("Error?  iRefs=%d for key ", i.getValue().iRefs );
					__tracer_format( i.getKey() );
					printf("!\n");
				}
				lStore.first()->unload(
					i.getValue().pData,
					i.getKey()
					);
			}
			for( typename StoreList::iterator i = lStore.begin();
				 i != lStore.end(); i++ )
			{
				delete *i;
			}
		}

		void appendStore( Store *pHand )
		{
			TRACE();
			lStore.append( pHand );
		}

		void prependStore( Store *pHand )
		{
			TRACE();
			lStore.prepend( pHand );
		}

		Ptr insert( obtype *pData )
		{
			TRACE( pData );
			CacheEntry e = {pData, 0};
			keytype k = lStore.first()->create( pData );
			hEnt.insert( k, e );

			return Ptr( *this, pData, k );
		}

		Ptr get( keytype cId )
		{
			TRACE( cId );
			try {
				return Ptr( *this, hEnt.get( cId ).pData, cId );
			}
			catch( Bu::HashException &e ) {
				CacheEntry e = {lStore.first()->load( cId ), 0};
				hEnt.insert( cId, e );
				return Ptr( *this, e.pData, cId );
			}
		}

		void erase( keytype cId )
		{
			TRACE( cId );
			try {
				if( hEnt.get( cId ).iRefs > 0 )
				{
					printf("Shouldn't delete, references still exist!\n");
					return;
				}
			}
			catch( Bu::HashException &e ) {
				get( cId );
			}
			lStore.first()->destroy( hEnt.get( cId ).pData, cId );
			hEnt.erase( cId );
		}

		int getRefCnt( keytype cId )
		{
			TRACE( cId );
			return hEnt.get( cId ).iRefs;
		}

	private:
		void incRef( keytype cId )
		{
			TRACE( cId );
			hEnt.get( cId ).iRefs++;
		}

		void decRef( keytype cId )
		{
			TRACE( cId );
			CacheEntry &e = hEnt.get( cId );
			e.iRefs--;
		}

	private:
		CidHash hEnt;
		StoreList lStore;
	};
};

#endif
