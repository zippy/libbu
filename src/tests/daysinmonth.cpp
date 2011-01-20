/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"
#include "bu/util.h"

using namespace Bu;

int main()
{
	for( int j = 0; j < 12; j++ )
	{
		sio << "2012-" << j << " = "
			<< getDaysInMonth( j, 2012 ) << sio.nl;
	}

	return 0;
}

