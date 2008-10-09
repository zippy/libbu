#include "bu/cachable.h"

Bu::Cachable::Cachable()
{
}

Bu::Cachable::~Cachable()
{
}

template<> long Bu::getCacheId<Bu::Cachable>( const Bu::Cachable *o )
{
	return o->getCacheId();
}

