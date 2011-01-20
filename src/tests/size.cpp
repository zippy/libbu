/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/hash.h"
#include "bu/string.h"

#define pSize( t ) printf("%15s: %db\n", #t, sizeof( t ) );

int main()
{
	typedef Bu::Hash<char, char> charcharHash;
	typedef Bu::Hash<Bu::String, Bu::String> strstrHash;
	pSize( Bu::String );
	pSize( charcharHash );
	pSize( strstrHash );
}
