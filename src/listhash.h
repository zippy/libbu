/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_LIST_HASH_H
#define BU_LIST_HASH_H

#include "bu/hash.h"
#include "bu/list.h"

namespace Bu
{
	template<typename key, typename value, typename sizecalc = __calcNextTSize_fast, typename keyalloc = std::allocator<key>, typename valuealloc = std::allocator<Bu::List<value> >, typename challoc = std::allocator<uint32_t> >
	class ListHash : public Hash<key, Bu::List<value>, sizecalc, keyalloc, valuealloc, challoc>
	{
		typedef Hash<key, Bu::List<value>, sizecalc, keyalloc, valuealloc, challoc> ParentType;
	public:
		ListHash()
		{
		}

		ListHash( const ListHash &src ) :
			ParentType( src )
		{
		}

		virtual ~ListHash()
		{
		}

		ListHash &operator=( const ListHash &src )
		{
			*dynamic_cast<ParentType *>(this) =
				dynamic_cast<ParentType &>(src);
		}

		virtual void insert( const key &k, const value &v )
		{
			if( !has( k ) )
			{
				ParentType::insert( k, List<value>() );
			}
			get( k ).append( v );
		}
		
	private:
	};

};

#endif
