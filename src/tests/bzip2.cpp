/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/bzip2.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
	if( argc < 3 )
	{
		printf("usage:  %s <in> <out>\n", argv[0] );
		return -1;
	}

	char buf[1024];
	size_t nRead;

	Bu::File f( argv[0], Bu::File::Write );
	Bu::BZip2 bz2( f );

	Bu::File fin( argv[1], Bu::File::Read );

	for(;;)
	{
		nRead = fin.read( buf, 1024 );
		if( nRead > 0 )
			bz2.write( buf, nRead );
		if( fin.isEos() )
			break;
	}
}

