#ifndef BU_C_PTR_H
#define BU_C_PTR_H

namespace Bu
{
	template<class obtype> class Cache;

	template<class obtype>
	class CPtr
	{
	friend class Bu::Cache<obtype>;
	private:
		CPtr( Cache<obtype> &rCache, obtype *pData ) :
			rCache( rCache ),
			pData( pData )
		{
			rCache.incRef( pData );
		}

	public:
		virtual ~CPtr()
		{
			rCache.decRef( pData );
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
	};
};

#endif
