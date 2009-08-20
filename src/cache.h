/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
//	template<class obtype, class keytype>
//	keytype __cacheGetKey( obtype *&pObj );

	template<class obtype, class keytype>
	class Cache
	{
	public:
		/**
		 * Cache Pointer - Provides access to data that is held within the
		 * cache.  This provides safe, refcounting access to data stored in
		 * the cache, with support for lazy loading.
		 */
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
			
			Ptr( Cache<obtype, keytype> *pCache, const keytype &kId ) :
				pCache( pCache ),
				pData( NULL ),
				kId( kId )
			{
			}

		public:
			Ptr( const Ptr &rSrc ) :
				pCache( rSrc.pCache ),
				pData( rSrc.pData ),
				kId( rSrc.kId )
			{
				if( pCache && pData )
					pCache->incRef( kId );
			}

			Ptr() :
				pCache( 0 ),
				pData( 0 )
			{
			}

			virtual ~Ptr()
			{
				if( pCache && pData )
					pCache->decRef( kId );
			}

			obtype &operator*()
			{
				checkPtr();
				return *pData;
			}

			const obtype &operator*() const
			{
				checkPtr();
				return *pData;
			}

			obtype *operator->()
			{
				checkPtr();
				return pData;
			}

			const obtype *operator->() const
			{
				checkPtr();
				return pData;
			}

			bool isValid() const
			{
				return pCache != NULL;
			}

			bool isBound() const
			{
				return pData != NULL;
			}

			operator bool() const
			{
				return isBound() && isValid();
			}

			const keytype &getKey() const
			{
				return kId;
			}

			void unbind()
			{
				if( pCache && pData )
					pCache->decRef( kId );
				pData = NULL;
			}

			void clear()
			{
				unbind();
				pCache = NULL;
			}

			void unset()
			{
				clear();
			}

			Ptr &operator=( const Ptr &rRhs )
			{
				if( pCache && pData )
					pCache->decRef( kId );
				pCache = rRhs.pCache;
				pData = rRhs.pData;
				kId = rRhs.kId;
				if( pCache && pData )
					pCache->incRef( kId );
				return *this;
			}

		private:
			void checkPtr() const
			{
				if( pCache && !pData )
				{
					pData = pCache->getRaw( kId );
					pCache->incRef( kId );
				}
			}

		private:
			Bu::Cache<obtype, keytype> *pCache;
			mutable obtype *pData;
			mutable keytype kId;
		};

	private:
		typedef Bu::CacheStore<obtype, keytype> Store;
		typedef Bu::List<Store *> StoreList;
		typedef Bu::CacheCalc<obtype, keytype> Calc;
		
		typedef struct CacheEntry
		{
			obtype *pData;
			int iRefs;
			time_t tLastSync;
		} CacheEntry;

		typedef Bu::Hash<keytype, CacheEntry> CidHash;

	public:
		Cache( Calc *pCalc, Store *pStore ) :
			pCalc( pCalc ),
			pStore( pStore )
		{
			TRACE();
			pCalc->setCache( this );
		}

		virtual ~Cache()
		{
			TRACE();
			for( typename CidHash::iterator i = hEnt.begin();
				 i != hEnt.end(); i++ )
			{
				if( i.getValue().iRefs > 0 )
				{
					// TODO: Throw an error in this case? iRefs != 0 for an
					// object when the Cache is destroyed.
					throw Bu::ExceptionBase("iRefs not zero.");
				}
				pCalc->onUnload(
					i.getValue().pData,
					i.getKey()
					);
				pStore->unload(
					i.getValue().pData,
					i.getKey()
					);
			}
			delete pCalc;
			delete pStore;
		}

		Ptr insert( obtype *pData )
		{
			TRACE( pData );
			CacheEntry e = {pData, 0, 0};
			keytype k = pStore->create( pData );
			hEnt.insert( k, e );

			pCalc->onLoad( pData, k );

			return Ptr( this, pData, k );
		}

		bool has( const keytype &cId )
		{
			return hEnt.has( cId ) || pStore->has( cId );
		}

		/**
		 * Retrieve an object from the cache and return a pointer to it.
		 * The object returned may be loaded from backend storage if needed,
		 * or the currently live object will be returned.
		 *@param cId The id of the object to load.
		 *@returns A pointer to the object.
		 */
		Ptr get( const keytype &cId )
		{
			TRACE( cId );
			try {
				return Ptr( this, hEnt.get( cId ).pData, cId );
			}
			catch( Bu::HashException &e ) {
				CacheEntry e = {pStore->load( cId ), 0, time( NULL )};
				pCalc->onLoad( e.pData, cId );
				hEnt.insert( cId, e );
				return Ptr( this, e.pData, cId );
			}
		}

		/**
		 * Retrieve a handle to an object without loading it now.  This function
		 * will return a pointer that has not yet been "realized" but can be
		 * used normally.  Upon initial use in any way the object will be
		 * loaded from the cache, either linking against the already loaded
		 * object or loading it fresh from the backend storage.  The advantage
		 * of this is that you recieve a usable handle to the data, but it
		 * does not count as a reference yet, meaning that the data is loaded
		 * when you need it, not before.
		 */
		Ptr getLazy( const keytype &cId )
		{
			TRACE( cId );
			return Ptr( this, cId );
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
			pCalc->onUnload( pObj, cId );
			hEnt.erase( cId );

			// The unload has to happen last just in case cId is a reference
			// to data that is about to be deleted from memory by the unload.
			pStore->unload( pObj, cId );
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
			
			pCalc->onUnload( hEnt.get( cId ).pData, cId );
			
			pStore->destroy( hEnt.get( cId ).pData, cId );
			hEnt.erase( cId );
		}

		typedef Bu::List<keytype> KeyList;
		KeyList getKeys()
		{
			return pStore->getKeys();
		}

		int getSize()
		{
			return pStore->getSize();
		}

		/**
		 * Make sure all currently loaded but not-in-use objects are synced to
		 * the store.
		 */
		void sync()
		{
			TRACE();
			int iSynced = 0;
			for( typename CidHash::iterator i = hEnt.begin();
				 i != hEnt.end(); i++ )
			{
				if( i.getValue().iRefs == 0 )
				{
					if( pCalc->shouldSync(
							i.getValue().pData,
							i.getKey(),
							i.getValue().tLastSync
							) )
					{
						pStore->sync(
							i.getValue().pData,
							i.getKey()
							);
						iSynced++;
						i.getValue().tLastSync = time( NULL );
					}
				}
			}
			if( iSynced > 0 )
			{
				pStore->sync();
			}
		}

	private:
		void incRef( const keytype &cId )
		{
			TRACE( cId );
			hEnt.get( cId ).iRefs++;
		}

		void decRef( const keytype &cId )
		{
			TRACE( cId );
			CacheEntry &e = hEnt.get( cId );
			e.iRefs--;
		}

		obtype *getRaw( const keytype &cId )
		{
			TRACE( cId );
			try {
				return hEnt.get( cId ).pData;
			}
			catch( Bu::HashException &e ) {
				CacheEntry e = {pStore->load( cId ), 0, time( NULL )};
				pCalc->onLoad( e.pData, cId );
				hEnt.insert( cId, e );
				return e.pData;
			}
		}

	private:
		CidHash hEnt;
		Calc *pCalc;
		Store *pStore;
	};
};

#endif
