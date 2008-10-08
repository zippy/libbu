#ifndef BU_C_PTR_H
#define BU_C_PTR_H

#include "bu/cache.h"

namespace Bu
{
	template<class obtype>
	class CPtr
	{
	friend class Bu::Cache<obtype>;
	private:
		CPtr( Cache<obtype> &rCache, obtype &rData )
		{
		}

	public:
		virtual ~CPtr()
		{
		}
	};
};

#endif
