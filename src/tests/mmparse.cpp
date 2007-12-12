/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/minimacro.h"
#include "bu/fstring.h"

int main( int argc, char *argv[] )
{
	Bu::MiniMacro mm;

	mm.addVar("name", "Mike");
	mm.addVar("age", "no");

	printf("%s\n", mm.parse("Hey there {=name:toupper():tolower()}, how are you?").getStr() );

	return 0;
}
