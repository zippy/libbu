#ifndef BU_CACHE_HANDLER_H
#define BU_CACHE_HANDLER_H

#include "bu/cptr.h"

namespace Bu
{
	/**
	 * Handles I/O for data in the cache.  This also assigns ID's to the newly
	 * created objects that are requested through this system.
	 */
	template<class T>
	class CacheHandler
	{
	public:
		CacheHandler()
		{
		}

		virtual ~CacheHandler()
		{
		}

		virtual Bu::CPtr<T> load()=0;
		virtual void unload( Bu::CPtr<T> pObj )=0;
		virtual Bu::CPtr<T> create()=0;
		virtual Bu::CPtr<T> create( T &rSrc )=0;
		virtual void destroy( Bu::CPtr<T> pObj )=0;

	private:
	};
};

#endif
