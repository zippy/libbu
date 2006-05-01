#include "httpconnectionmonitor.h"
#include "http.h"
#include <sys/stat.h>

HttpConnectionMonitor::HttpConnectionMonitor()
{
}

HttpConnectionMonitor::~HttpConnectionMonitor()
{
}

bool HttpConnectionMonitor::onNewConnection( Connection *pCon )
{
	Http hp( pCon );

	pCon->readInput( 60, 0 );
	printf("#######################\n%s\n#######################\n", pCon->getInput() );

	while( hp.parseRequest() == false );
	printf("Done parsing.\n\n");

	if( hp.getRequestType() == Http::reqGet )
	{
		printf("\"\"\"%s\"\"\"\n", hp.getRequestURI() );
		if( !strcmp( hp.getRequestURI(), "/" ) )
		{
			std::string content("<html><head><title>Server Test</test></head><body>This is a test of a new system where all the pages will be more or less dynamic...<br>If you want to try to login, you can do that here:<br><form method=\"post\" action=\"showvars\"  enctype=\"multipart/form-data\">Name: <input type=\"text\" name=\"name\"><br>Password: <input type=\"password\" name=\"pass\"><br><input type=\"submit\" name=\"action\" value=\"login\"></form></body></html>");
			hp.buildResponse();
			hp.setResponseContent(
				"text/html",
				content.c_str(),
				content.size()
				);
			hp.sendResponse();
		}
		else
		{
			std::string content("<html><head><title>URL Not Found</test></head><body>There is no content mapped to the URL you requested.  Please try another one.</body></html>");
			hp.buildResponse( 404, "File not found.");
			hp.setResponseContent(
				"text/html",
				content.c_str(),
				content.size()
				);
			hp.sendResponse();
		}
	}
	else
	{
		printf("Non get:  %s\n", hp.getRequestTypeStr() );
	}
	pCon->writeOutput();
	
	if( pCon->hasInput() )
	{
		std::string s( pCon->getInput(), pCon->getInputAmnt() );
		
		printf("Reamining data\n==============\n%s\n==============\n",
			s.c_str() );
	}

	pCon->disconnect();

	return true;
}

bool HttpConnectionMonitor::onClosedConnection( Connection *pCon )
{
	return true;
}

