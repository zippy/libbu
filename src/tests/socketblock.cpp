/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/ito.h"
#include "bu/tcpsocket.h"
#include "bu/tcpserversocket.h"
#include <stdio.h>
#include <unistd.h>

class TstServer : public Bu::Ito
{
public:
	TstServer() :
		s( 55678 )
	{
	}

	virtual void run()
	{
		Bu::TcpSocket c = s.accept( 45, 0 );
		printf("TstServer:  Accetped connection.\n"); fflush( stdout );
		
		sleep( 1 );
		printf("TstServer:  Trying to read 10 bytes...\n"); fflush( stdout );

		char buf[10];
		size_t nRead = c.read( buf, 10 );
		printf("TstServer:  Got %d bytes...\n", nRead ); fflush( stdout );

		printf("TstServer:  Closing connection...\n"); fflush( stdout );
		c.close();
	}

	Bu::TcpServerSocket s;
};

int main()
{
	TstServer ts;

	ts.start();

	printf("main:  Connecting to server.\n"); fflush( stdout );
	Bu::TcpSocket s( "localhost", 55678 );

	printf("main:  Sending 4 bytes.\n"); fflush( stdout );
	s.write( "aoeu", 4 );

	printf("main:  Sleeping 10 seconds for good measure.\n"); fflush( stdout );
	sleep( 10 );
}

