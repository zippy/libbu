#ifndef HTTPCONNECTIONMONITOR_H
#define HTTPCONNECTIONMONITOR_H

#include "connectionmonitor.h"

class HttpConnectionMonitor : public ConnectionMonitor
{
public:
	HttpConnectionMonitor();
	~HttpConnectionMonitor();
	
	bool onNewConnection( Connection *pCon, int nPort );
	bool onClosedConnection( Connection *pCon );
};

#endif
