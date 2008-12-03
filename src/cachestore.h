#ifndef BU_CACHE_STORE_H
#define BU_CACHE_STORE_H

namespace Bu
{
	/**
	 * Handles I/O for data in the cache.  This also assigns ID's to the newly
	 * created objects that are requested through this system.
	 */
	template<class obtype, class keytype>
	class CacheStore
	{
	public:
		CacheStore()
		{
		}

		virtual ~CacheStore()
		{
		}

		virtual obtype *load( const keytype &key )=0;
		virtual void unload( obtype *pObj, const keytype &key )=0;
		virtual keytype create( obtype *pSrc )=0;
		virtual void destroy( obtype *pObj, const keytype &key )=0;

	private:
	};
};

#endif
