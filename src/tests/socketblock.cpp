/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/ito.h"
#include "bu/socket.h"
#include "bu/serversocket.h"
#include <stdio.h>

class TstServer : public Bu::Ito
{
public:
	TstServer() :
		s( 55678 )
	{
	}

	virtual void *run()
	{
		Bu::Socket c = s.accept( 45, 0 );
		printf("TstServer:  Accetped connection.\n"); fflush( stdout );
		
		sleep( 1 );
		printf("TstServer:  Trying to read 10 bytes...\n"); fflush( stdout );

		char buf[10];
		size_t nRead = c.read( buf, 10 );
		printf("TstServer:  Got %d bytes...\n", nRead ); fflush( stdout );

		printf("TstServer:  Closing connection...\n"); fflush( stdout );
		c.close();

		return NULL;
	}

	Bu::ServerSocket s;
};

int main()
{
	TstServer ts;

	ts.start();

	printf("main:  Connecting to server.\n"); fflush( stdout );
	Bu::Socket s( "localhost", 55678 );

	printf("main:  Sending 4 bytes.\n"); fflush( stdout );
	s.write( "aoeu", 4 );

	printf("main:  Sleeping 10 seconds for good measure.\n"); fflush( stdout );
	sleep( 10 );
}

