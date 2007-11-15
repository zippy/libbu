/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/bzip2.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
	char buf[1024];
	size_t nRead;

	Bu::File f( "test.bz2", "wb" );
	Bu::BZip2 bz2( f );

	Bu::File fin( argv[1], "rb");

	for(;;)
	{
		nRead = fin.read( buf, 1024 );
		if( nRead > 0 )
			bz2.write( buf, nRead );
		if( fin.isEOS() )
			break;
	}
}

