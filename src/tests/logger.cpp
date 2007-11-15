/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/logger.h"
#include <errno.h>
#include <stdlib.h>

class Thing
{
	public:
		Thing()
		{
			lineLog( 2, "Want a thing?");
		}

		void go( int i )
		{
			lineLog( 1, "GO!!!!");
		}
};

int main()
{
	setLogLevel( 4 );
	setLogFormat("%L: %y-%02m-%02d %h:%02M:%02s %f:%l:%F: %t");
	lineLog( 5, "Hey, error: %s", strerror( errno ) );

	logHexDump( 5, "This is a test of the hex-dump facility", 16, "Random stuff");

	Thing gh;
	gh.go( 6);
}

