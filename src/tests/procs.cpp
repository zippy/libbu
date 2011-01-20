/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/process.h"

#include <stdio.h>

int main()
{
    Bu::Process p( Bu::Process::Both, "/bin/bash", "/bin/bash", "-c", "echo Hello 1>&2; echo StdOut; sleep 1; echo Yup; echo Yup 1>&2", NULL );

	char buf[1000];
	while( !p.isEos() )
	{
		bool out, err;
		p.select( out, err );
		if( out )
		{
			int iSize = p.read( buf, 1000 );
			printf("::read=%d::\n", iSize );
			fwrite( buf, iSize, 1, stdout );
		}
		if( err )
		{
			int iSize = p.readErr( buf, 1000 );
			printf("::readErr=%d::\n", iSize );
			fwrite( buf, iSize, 1, stdout );
		}
	}

	return 0;
}

