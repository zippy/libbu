#include "bu/serversocket.h"
#include "bu/socket.h"

int main()
{
	Bu::ServerSocket sSrv( 9987 );

	Bu::Socket sSend("localhost", 9987 );

	Bu::Socket sRecv( sSrv.accept() );

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
