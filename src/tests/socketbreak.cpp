/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tcpserversocket.h"
#include "bu/tcpsocket.h"
#include <unistd.h>

int main()
{
	Bu::TcpServerSocket sSrv( 9987 );

	Bu::TcpSocket sSend("localhost", 9987 );

	Bu::TcpSocket sRecv( sSrv.accept() );

	printf("Connected sockets.\n");

	sleep( 1 );
	printf("Closing sRecv.\n");
	sRecv.close();
	sleep( 1 );

	char buf[3];
	printf("About to write.\n");
	printf("write:  %d\n", sSend.write("hi", 2 ) );
	printf("About to read.\n");
	printf("read:   %d\n", sSend.read( buf, 2 ) );

	return 0;
}

