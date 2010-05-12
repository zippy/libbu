/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archive.h"
#include "bu/file.h"
#include "bu/fstring.h"

using namespace Bu;

int main()
{
	File f("test.dat", File::WriteNew );
	Archive ar( f, Archive::save );

	Bu::FString s("Hello there");
	ar << s;

	ar.setProp("hi", 45 );
	printf("Hi: %d", ar.getProp<int>("hi") );

	return 0;
}

