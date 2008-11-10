#ifndef BU_CACHE_HANDLER_H
#define BU_CACHE_HANDLER_H

namespace Bu
{
	class CacheHandler
	{
	public:
		CacheHandler();
		virtual ~CacheHandler();
		virtual void load()=0;
		virtual void unload()=0;

	private:
	};
};

#endif
