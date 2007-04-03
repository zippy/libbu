#include "connectionmanager.h"
#include "httpconnectionmonitor.h"

int main()
{
	printf("Starting server...\n");

	ConnectionManager srv;
	HttpConnectionMonitor http;

	srv.setConnectionMonitor( &http );

	printf("Listening on port 7331\n");
	srv.startServer( 7331 );

	for(;;)
	{
		srv.scanConnections( 5000, false );
	}
	
	return 0;
}
