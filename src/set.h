/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SET_H
#define BU_SET_H

#include <stddef.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <list>
#include <utility>
#include "bu/exceptionbase.h"
#include "bu/list.h"
#include "bu/archive.h"

#define bitsToBytes( n ) (n/32+(n%32>0 ? 1 : 0))

namespace Bu
{
	subExceptionDecl( SetException )

	/**
	 *@todo Set should be rewritten, possibly using a b-tree as ordered storage
	 * in the backend.  It should use either a b-tree or array for storage and
	 * allow set intersections, unions, etc.
	 *@param key (typename) The datatype of the hashtable keys
	 *@param sizecalc (typename) Functor to compute new table size on rehash
	 *@param keyalloc (typename) Memory allocator for hashtable keys
	 *@param challoc (typename) Byte allocator for bitflags
	 *@ingroup Containers
	 */
	template<typename key, typename sizecalc, typename keyalloc, typename challoc >
	class Set
	{
	public:
	};
}

#endif
