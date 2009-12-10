/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/process.h"

#include <stdio.h>

int main()
{
	//Bu::Process p( argv[1], argv+1 );
	    Bu::Process p("mplayer", "mplayer", "dvd://", "-framedrop",
        "-ao", "null", "-nosound", "-vf", "framestep=I,cropdetect", "-sstep",
        "197", NULL );

	char buf[1000];
	for(;;)
	{
		int iSize = p.read( buf, 1000 );
		printf("::read=%d::\n", iSize );
		if( iSize == 0 )
			break;
		fwrite( buf, iSize, 1, stdout );
		if( iSize < 1000 )
			break;
	}

	return 0;
}

