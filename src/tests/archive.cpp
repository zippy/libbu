/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archive.h"
#include "bu/file.h"

using namespace Bu;

int main()
{
	File f("test.dat", "wb");
	Archive ar( f, Archive::save );

	std::string s("Hello there");
	ar << s;

	return 0;
}

