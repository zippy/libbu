#ifndef BU_CACHABLE_H
#define BU_CACHABLE_H

namespace Bu
{
	class Cachable
	{
	public:
		Cachable();
		virtual ~Cachable();

		virtual long getCacheId() const =0;
	};

	template<class obtype> long getCacheId( const obtype *o );
	template<> long getCacheId<Cachable>( const Cachable *o );
};

#define DECL_CACHABLE( name ) \
	namespace Bu { template<> long getCacheId<name>( const name *o ); }

#define DEF_CACHABLE( name ) \
	template<> long Bu::getCacheId<name>( const name *o ) \
	{	\
		return getCacheId<Bu::Cachable>( (Bu::Cachable *)o );	\
	}

#endif
