/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/crypt.h"
#include "bu/sio.h"

using namespace Bu;

int main( int argc, char *argv[] )
{
	if( argc == 1 )
		sio << "Syntax: " << argv[0] << " <password> [salt]" << sio.nl
			<< sio.nl;
	else if( argc == 2 )
		sio << "Crypt1:  >> " << cryptPass( argv[1] ) << " <<" << sio.nl;
	else
		sio << "Crypt2:  >> " << cryptPass( argv[1], argv[2] ) << " <<" << sio.nl;

	return 0;
}

