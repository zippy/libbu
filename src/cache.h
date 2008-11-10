#ifndef BU_CACHE_H
#define BU_CACHE_H

#include "bu/cptr.h"
#include "bu/hash.h"
#include "bu/list.h"
#include "bu/cachehandler.h"

#define BU_TRACE
#include "bu/trace.h"

namespace Bu
{
	template<class obtype> class Cache;
	template<class obtype> long getCacheId( const obtype *o );

	template<class obtype>
	class Cache
	{
	friend class Bu::CPtr<obtype>;
	typedef Bu::CPtr<obtype> Ptr;
	public:
		typedef long cid;	/**< Cache ID type. Unique cache entry ID. */

	public:
		Cache()
		{
			TRACE();
		}

		virtual ~Cache()
		{
			TRACE();
			for( HandlerList::iterator i = lHandler.begin();
				 i != lHandler.end(); i++ )
			{
				delete *i;
			}
		}

		void appendHandler( CacheHandler *pHand )
		{
			lHandler.append( pHand );
		}

		void prependHandler( CacheHandler *pHand )
		{
			lHandler.prepend( pHand );
		}

		Ptr insert( obtype *pData )
		{
			TRACE();
			CacheEntry e = {pData, 0};
			hEnt.insert( getCacheId( pData ), e );
			return Ptr( *this, pData );
		}

		Ptr get( cid cId )
		{
			TRACE();
			return Ptr( *this, hEnt.get( cId ).pData );
		}

		int getRefCnt( cid cId )
		{
			TRACE();
			return hEnt.get( cId ).iRefs;
		}

	private:
		void incRef( obtype *pData )
		{
			TRACE();
			hEnt.get( getCacheId( pData ) ).iRefs++;
		}

		void decRef( obtype *pData )
		{
			TRACE();
			CacheEntry &e = hEnt.get( getCacheId( pData ) );
			e.iRefs--;
		}

	private:
		typedef struct CacheEntry
		{
			obtype *pData;
			int iRefs;
		} CacheEntry;

		//typedef Bu::Hash<ptrdiff_t, int> RefHash;
		typedef Bu::Hash<cid, CacheEntry> CidHash;
		//RefHash hRefs;
		CidHash hEnt;
		typedef Bu::List<CacheHandler *> HandlerList;
		HandlerList lHandler;
	};
};

#endif
