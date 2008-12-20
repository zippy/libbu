#ifndef BU_CACHE_CALC_H
#define BU_CACHE_CALC_H

#include "bu/trace.h"

namespace Bu
{
	template<class obtype, class keytype> class Cache;

	template<class obtype, class keytype>
	class CacheCalc
	{
	friend class Cache<obtype, keytype>;
	private:
		typedef Cache<obtype, keytype> MyCache;
	public:
		CacheCalc() :
			pCache( (MyCache *)0 )
		{
			TRACE();
		}

		virtual ~CacheCalc()
		{
			TRACE();
		}

		virtual void onLoad( obtype *pSrc, const keytype &key )=0;
		virtual void onUnload( obtype *pSrc, const keytype &key )=0;
		virtual void onTick() { };

	protected:
		MyCache *getCache()
		{
			TRACE();
			return pCache;
		}

	private:
		void setCache( MyCache *pCache )
		{
			TRACE();
			this->pCache = pCache;
		}

		MyCache *pCache;
	};
};

#endif
