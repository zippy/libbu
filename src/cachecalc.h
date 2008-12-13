#ifndef BU_CACHE_CALC_H
#define BU_CACHE_CALC_H

namespace Bu
{
	template<class obtype, class keytype>
	class CacheCalc
	{
	public:
		CacheCalc()
		{
		}

		virtual ~CacheCalc()
		{
		}

		virtual void onLoad( obtype *pSrc, const keytype &key )=0;
		virtual void onUnload( obtype *pSrc, const keytype &key )=0;
		virtual void onTick() { };

	private:
	};
};

#endif
