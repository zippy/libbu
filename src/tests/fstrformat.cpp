/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/string.h"
#include <stdio.h>

int main()
{
	Bu::String s;

	s.format("%d, %f, \'%s\'", 144, 12.5, "bob" );

	printf("test:  %s\n", s.getStr() );
}

