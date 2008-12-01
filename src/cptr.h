#ifndef BU_C_PTR_H
#define BU_C_PTR_H

namespace Bu
{
	template<class obtype, class keytype> class Cache;

	/**
	 * Cache Pointer - Provides access to data that is held within the cache.
	 * This provides safe, refcounting access to data stored in the cache, with
	 * support for lazy loading.
	 */
	template<class obtype, class keytype>
	class CPtr
	{
	friend class Bu::Cache<obtype, keytype>;
	private:
		CPtr( Cache<obtype, keytype> &rCache, obtype *pData ) :
			rCache( rCache ),
			pData( pData )
		{
			rCache.incRef( kId );
		}

	public:
		virtual ~CPtr()
		{
			rCache.decRef( kId );
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
		Bu::Cache<obtype, keytype> &rCache;
		obtype *pData;
		keytype kId;
	};
};

#endif
