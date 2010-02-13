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
	File f("test.dat", File::Write );
	Archive ar( f, Archive::save );

	Bu::FString s("Hello there");
	ar << s;

	return 0;
}

