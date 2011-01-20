/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/url.h"

#include <stdio.h>

int main( int argc, char *argv[] )
{
	for( argc--, argv++; argc >= 0; argc--, argv++ )
	{
		printf("encodede: %s\n", Bu::Url::encode( *argv ).getStr() );
		printf("decodede: %s\n", Bu::Url::decode( *argv ).getStr() );
		Bu::Url u( *argv );

		printf("Protocol: %s\n", u.getProtocol().getStr() );
		printf("User:     %s\n", u.getUser().getStr() );
		printf("Pass:     %s\n", u.getPass().getStr() );
		printf("Host:     %s\n", u.getHost().getStr() );
		printf("Path:     %s\n", u.getPath().getStr() );
		try
		{
			printf("Port:     %d\n", u.getPort() );
		} catch( Bu::ExceptionBase &e )
		{
			printf("Port:     not set.\n");
		}
		printf("Parameters:\n");
		for( Bu::Url::ParamList::const_iterator i = u.getParamBegin(); i; i++ )
		{
			printf("  \"%s\" = \"%s\"\n",
				(*i).sName.getStr(), (*i).sValue.getStr()
				);
		}
	}

	return 0;
}
