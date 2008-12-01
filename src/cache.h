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
			for( typename StoreList::iterator i = lStore.begin();
				 i != lStore.end(); i++ )
			{
				delete *i;
			}
		}

		void appendStore( Store *pHand )
		{
			lStore.append( pHand );
		}

		void prependStore( Store *pHand )
		{
			lStore.prepend( pHand );
		}

		Ptr insert( obtype *pData )
		{
			TRACE();
			CacheEntry e = {pData, 0};
			hEnt.insert( 0 , e );
			return Ptr( *this, pData );
		}

		Ptr get( keytype cId )
		{
			TRACE();
			return Ptr( *this, hEnt.get( cId ).pData );
		}

		int getRefCnt( keytype cId )
		{
			TRACE();
			return hEnt.get( cId ).iRefs;
		}

	private:
		void incRef( keytype cId )
		{
			TRACE();
			hEnt.get( cId ).iRefs++;
		}

		void decRef( keytype cId )
		{
			TRACE();
			CacheEntry &e = hEnt.get( cId );
			e.iRefs--;
		}

	private:
		CidHash hEnt;
		StoreList lStore;
	};
};

#endif
