/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/sio.h>
using namespace Bu;

#include <iostream>
using namespace std;

typedef struct Counter
{
	Counter() : i( 0 )
	{
	}

	int get()
	{
		i++;
		return i-1;
	}

	int i;
} Counter;

template<typename a>
void runtest( a &out )
{
	Counter c;
	out << c.get() << ", " << c.get() << ", " << c.get() << ", " << c.get() << ", " << c.get() << "\n";
}

int main()
{
	runtest( cout );
	runtest( sio );

	return 0;
}

