/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ATOM_H
#define BU_ATOM_H

#include <stdint.h>
#include <memory>
#include "bu/exceptions.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Containers
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

		Atom( const MyType &oth ) :
			pData( NULL )
		{
			if( oth.pData )
				set( *oth.pData );
		}

		Atom( const t &oth ) :
			pData( NULL )
		{
			set( oth );
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
		
		MyType &operator =( const MyType &oth )
		{
			if( oth.pData )
				set( *oth.pData );

			return *this;
		}
		
		bool operator ==( const MyType &oth )
		{
			return (*pData) == (*oth.pData);
		}

		bool operator ==( const t &oth )
		{
			return (*pData) == oth;
		}

		t *operator ->()
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return pData;
		}

		t &operator *()
		{
			if( !pData )
				throw Bu::ExceptionBase("Not set");
			return *pData;
		}

	private:
		t *pData;
		talloc ta;
	};
}

#endif
