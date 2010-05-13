/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/list.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	List<int64_t> lInt;

	lInt.append( 512 );
	lInt.append( 1024 );
	lInt.append( 4096 );
	lInt.append( 12 );
	lInt.erase( 12 );
	lInt.append( 12 );
	lInt.erase( 12 );
	lInt.append( 12 );
}

