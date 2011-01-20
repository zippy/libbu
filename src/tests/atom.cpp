/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/atom.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct bob
{
	int a, b;
} bob;
int main()
{
	Bu::Atom<int> aInt;
	Bu::Atom<const char *> aStr;
	Bu::Atom<bob> aBob;

	aBob = bob();
	aBob->a = 5;

	aStr.set("Hey there, dude");
	aInt.set( 55 );
	int me = aInt;
	aInt = 12;
	printf("%d, %d\n", aInt.get(), me );
	printf("%s\n", aStr.get() );
}

