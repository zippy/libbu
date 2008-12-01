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
	template<class obtype>
	class Cache
	{
	friend class Bu::CPtr<obtype>;
		typedef Bu::CPtr<obtype> Ptr;
		typedef Bu::CacheHandler<obtype> Handler;
		typedef Bu::List<Handler *> HandlerList;
	public:
		typedef long cid_t;	/**< Cache ID type. Unique cache entry ID. */

	public:
		Cache()
		{
			TRACE();
		}

		virtual ~Cache()
		{
			TRACE();
/*			for( HandlerList::iterator i = lHandler.begin();
				 i != lHandler.end(); i++ )
			{
				delete *i;
			}
*/		}

		void appendHandler( Handler *pHand )
		{
			lHandler.append( pHand );
		}

		void prependHandler( Handler *pHand )
		{
			lHandler.prepend( pHand );
		}

		Ptr insert( obtype *pData )
		{
			TRACE();
			CacheEntry e = {pData, 0};
			hEnt.insert( /*pData*/ 0 , e );
			return Ptr( *this, pData );
		}

		Ptr get( cid_t cId )
		{
			TRACE();
			return Ptr( *this, hEnt.get( cId ).pData );
		}

		int getRefCnt( cid_t cId )
		{
			TRACE();
			return hEnt.get( cId ).iRefs;
		}

	private:
		void incRef( cid_t cId )
		{
			TRACE();
			hEnt.get( cId ).iRefs++;
		}

		void decRef( cid_t cId )
		{
			TRACE();
			CacheEntry &e = hEnt.get( cId );
			e.iRefs--;
		}

	private:
		typedef struct CacheEntry
		{
			obtype *pData;
			int iRefs;
		} CacheEntry;

		//typedef Bu::Hash<ptrdiff_t, int> RefHash;
		typedef Bu::Hash<cid_t, CacheEntry> CidHash;
		//RefHash hRefs;
		CidHash hEnt;
		HandlerList lHandler;
	};
};

#endif
