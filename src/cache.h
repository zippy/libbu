#ifndef BU_CACHE_H
#define BU_CACHE_H

// #include "bu/cptr.h"
#include "bu/hash.h"
#include "bu/list.h"
#include "bu/cachestore.h"
#include "bu/cachecalc.h"

#include "bu/trace.h"

namespace Bu
{
	template<class obtype, class keytype>
	class Cache
	{
	//friend class Bu::CPtr<obtype, keytype>;
	public:
		// typedef Bu::CPtr<obtype, keytype> Ptr;

		/**
		 * Cache Pointer - Provides access to data that is held within the
		 * cache.  This provides safe, refcounting access to data stored in
		 * the cache, with support for lazy loading.
		 */
		//template<class obtype, class keytype>
		class Ptr
		{
		friend class Bu::Cache<obtype, keytype>;
		private:
			Ptr( Cache<obtype, keytype> *pCache, obtype *pData,
				const keytype &kId ) :
				pCache( pCache ),
				pData( pData ),
				kId( kId )
			{
				if( pCache )
					pCache->incRef( kId );
			}

		public:
			Ptr( const Ptr &rSrc ) :
				pCache( rSrc.pCache ),
				pData( rSrc.pData ),
				kId( rSrc.kId )
			{
				if( pCache )
					pCache->incRef( kId );
			}

			Ptr() :
				pCache( 0 ),
				pData( 0 )
			{
			}

			virtual ~Ptr()
			{
				if( pCache )
					pCache->decRef( kId );
			}

			obtype &operator*()
			{
				return *pData;
			}

			obtype *operator->()
			{
				return pData;
			}

			const keytype &getKey()
			{
				return kId;
			}

			Ptr &operator=( const Ptr &rRhs )
			{
				if( pCache )
					pCache->decRef( kId );
				pCache = rRhs.pCache;
				pData = rRhs.pData;
				kId = rRhs.kId;
				if( pCache )
					pCache->incRef( kId );
			}

		private:
			Bu::Cache<obtype, keytype> *pCache;
			obtype *pData;
			keytype kId;
		};

	private:
		typedef Bu::CacheStore<obtype, keytype> Store;
		typedef Bu::List<Store *> StoreList;
		typedef Bu::CacheCalc<obtype, keytype> Calc;
		
		typedef struct CacheEntry
		{
			obtype *pData;
			int iRefs;
		} CacheEntry;

		typedef Bu::Hash<keytype, CacheEntry> CidHash;

	public:
		Cache( Calc &rCalc ) :
			rCalc( rCalc )
		{
			TRACE();
			rCalc.setCache( this );
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
				rCalc.onUnload(
					i.getValue().pData,
					i.getKey()
					);
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

			rCalc.onLoad( pData, k );

			return Ptr( this, pData, k );
		}

		Ptr get( const keytype &cId )
		{
			TRACE( cId );
			try {
				return Ptr( this, hEnt.get( cId ).pData, cId );
			}
			catch( Bu::HashException &e ) {
				CacheEntry e = {lStore.first()->load( cId ), 0};
				rCalc.onLoad( e.pData, cId );
				hEnt.insert( cId, e );
				return Ptr( this, e.pData, cId );
			}
		}

		int getRefCount( const keytype &cId )
		{
			TRACE( cId );
			return hEnt.get( cId ).iRefs;
		}

		void unload( const keytype &cId )
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
				// It's not here?  Eh, return.
				return;
			}
			obtype *pObj = hEnt.get( cId ).pData;
			rCalc.onUnload( pObj, cId );
			hEnt.erase( cId );

			// The unload has to happen last just in case cId is a reference
			// to data that is about to be deleted from memory by the unload.
			lStore.first()->unload( pObj, cId );
		}

		void erase( const keytype &cId )
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
			
			rCalc.onUnload( hEnt.get( cId ).pData, cId );
			
			lStore.first()->destroy( hEnt.get( cId ).pData, cId );
			hEnt.erase( cId );
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
		Calc &rCalc;
	};
};

#endif
