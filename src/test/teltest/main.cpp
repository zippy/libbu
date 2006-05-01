#include "connectionmanager.h"
#include "telnetmonitor.h"

int main()
{
	printf("Starting server...\n");

	ConnectionManager srv;
	TelnetMonitor telnet;

	srv.setConnectionMonitor( &telnet );

	srv.startServer( 4001, 40 );

	for(;;)
	{
		srv.scanConnections( 5000, false );
	}
	
	return 0;
}
