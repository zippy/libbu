/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archive.h"
#include "bu/file.h"
#include "bu/string.h"

using namespace Bu;

int main()
{
	File f("test.dat", File::WriteNew );
	Archive ar( f, Archive::save );

	Bu::String s("Hello there");
	ar << s;

	ar.setProp("hi", 45 );
	printf("Hi: %d", ar.getProp<int>("hi") );

	return 0;
}

