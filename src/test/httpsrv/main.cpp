#include "connectionmanager.h"
#include "httpconnectionmonitor.h"

int main()
{
	printf("Starting server...\n");

	ConnectionManager srv;
	HttpConnectionMonitor http;

	srv.setConnectionMonitor( &http );

	srv.startServer( 7331, 40 );

	for(;;)
	{
		srv.scanConnections( 5000, false );
	}
	
	return 0;
}
