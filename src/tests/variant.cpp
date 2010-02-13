/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/sio.h>
#include <bu/variant.h>
#include <bu/list.h>

using namespace Bu;

Variant getThing( int i )
{
	Variant v;
	switch( i )
	{
	case 0:
		v = 45;
		break;

	case 1:
		v = true;
		break;

	case 2:
		v = List<int>(5).append(10).append(15);
		break;
	}

	return v;
}

int main()
{
	Variant a;
	Variant b;
	Variant c;

	a = getThing( 0 );
	b = getThing( 1 );
	c = getThing( 2 );

	sio << "a = " << a << " or " << (int)a << sio.nl
		<< "b = " << b << " or " << b.toString() << sio.nl
		<< "c = " << c << " or " << c.toString() << sio.nl
		<< sio.nl;

	return 0;
}

