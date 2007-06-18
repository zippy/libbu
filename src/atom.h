#ifndef ATOM_H
#define ATOM_H

#include <stdint.h>
#include <memory>
#include "bu/exceptions.h"

namespace Bu
{
	/**
	 *
	 */
	template <typename t, typename talloc=std::allocator<t> >
	class Atom
	{
	private:
		typedef struct Atom<t, talloc> MyType;

	public:
		Atom() :
			pData( NULL )
		{
		}

		virtual ~Atom()
		{
			clear();
		}

		bool has() const
		{
			return (pData != NULL);
		}

		void set( const t &val )
		{
			clear();
			pData = ta.allocate( 1 );
			ta.construct( pData, val );
		}

		t &get()
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return *pData;
		}

		const t &get() const
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return *pData;
		}

		void clear()
		{
			if( pData )
			{
				ta.destroy( pData );
				ta.deallocate( pData, 1 );
				pData = NULL;
			}
		}

		operator const t &() const
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return *pData;
		}
		
		operator t &()
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return *pData;
		}

		MyType &operator =( const t &oth )
		{
			set( oth );

			return *this;
		}

	private:
		t *pData;
		talloc ta;
	};
}

#endif
