#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "connection.h"

int main()
{
	Connection c;
	c.open("127.0.0.1", 12457 );

	{
		int newSocket = c.getSocket();
		int flags;

		flags = fcntl(newSocket, F_GETFL, 0);
		flags |= O_NONBLOCK;
		if (fcntl(newSocket, F_SETFL, flags) < 0)
		{
			return false;
		}
	}

	for( int i = 0; i < 50; i++ )
	{
		usleep( 100000 );
		int nbytes = c.readInput();
		if( nbytes == 0 )
			printf("0 bytes, EOF?\n");
		else
			printf("Got %d bytes, whacky...\n", nbytes );
	}

	c.close();

	return 0;
}

