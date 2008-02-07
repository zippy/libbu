/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/hash.h"
#include "bu/fstring.h"

#define pSize( t ) printf("%15s: %db\n", #t, sizeof( t ) );

int main()
{
	typedef Bu::Hash<char, char> charcharHash;
	typedef Bu::Hash<Bu::FString, Bu::FString> strstrHash;
	pSize( Bu::FString );
	pSize( charcharHash );
	pSize( strstrHash );
}
