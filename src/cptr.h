#ifndef BU_C_PTR_H
#define BU_C_PTR_H

namespace Bu
{
	template<class obtype> class Cache;

	/**
	 * Cache Pointer - Provides access to data that is held within the cache.
	 * This provides safe, refcounting access to data stored in the cache, with
	 * support for lazy loading.
	 */
	template<class obtype>
	class CPtr
	{
	friend class Bu::Cache<obtype>;
	public:
		typedef long cid_t;	/**< Cache ID type. Unique cache entry ID. */
	private:
		CPtr( Cache<obtype> &rCache, obtype *pData ) :
			rCache( rCache ),
			pData( pData )
		{
			rCache.incRef( cId );
		}

	public:
		virtual ~CPtr()
		{
			rCache.decRef( cId );
		}

		obtype &operator*()
		{
			return *pData;
		}

		obtype *operator->()
		{
			return pData;
		}

	private:
		Bu::Cache<obtype> &rCache;
		obtype *pData;
		cid_t cId;
	};
};

#endif
