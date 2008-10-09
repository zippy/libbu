#ifndef BU_CACHE_H
#define BU_CACHE_H

#include "bu/cptr.h"
#include "bu/hash.h"

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
		}

		Ptr insert( obtype *pData )
		{
			TRACE();
			CacheEntry e = {pData, 0};
			hEnt.insert( getCacheId( pData ), e );
			return Ptr( *this, pData );
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
	};
};

#endif
