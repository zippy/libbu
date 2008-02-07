/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <iostream>
#include "bu/fstring.h"

int main()
{
	Bu::FString s("Hey there, dude.\n");

	std::cout << s << 5;
}
