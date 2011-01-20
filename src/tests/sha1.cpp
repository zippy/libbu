/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"
#include "bu/file.h"
#include "bu/sha1.h"

using namespace Bu;

int main( int argc, char *argv[] )
{
	argv++, argc--;
	for(; *argv; argv++ )
	{
		Bu::File fIn( *argv, Bu::File::Read );
		Bu::Sha1 m;

		char buf[100000];
		for(;;)
		{
			int iRead = fIn.read( buf, 100000 );
			m.addData( buf, iRead );
			if( iRead < 100000 )
				break;
		}

		Bu::String sRes = m.getResult();
		for( Bu::String::iterator i = sRes.begin(); i; i++ )
		{
			sio << Fmt::hex(2,false) << (int)(unsigned char)(*i);
		}
		sio << " *" << *argv << sio.nl;
	}
}

