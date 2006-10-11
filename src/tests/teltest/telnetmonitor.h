#ifndef HTTPCONNECTIONMONITOR_H
#define HTTPCONNECTIONMONITOR_H

#include "connectionmonitor.h"
#include "programlink.h"
#include "linkedlist.h"

class TelnetMonitor : public ConnectionMonitor, public ProgramLink
{
public:
	TelnetMonitor();
	~TelnetMonitor();
	
	bool init();
	bool deInit();
	bool timeSlice();
	LinkMessage* processIRM( LinkMessage *pMsgIn );
	
	bool onNewConnection( Connection *pCon, int nPort );
	bool onClosedConnection( Connection *pCon );

private:
	LinkedList lCon;
};

#endif
