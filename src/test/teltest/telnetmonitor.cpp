#include "telnetmonitor.h"
#include "protocoltelnet.h"
#include <sys/stat.h>

TelnetMonitor::TelnetMonitor()
{
}

TelnetMonitor::~TelnetMonitor()
{
}

bool TelnetMonitor::init()
{
	return true;
}

bool TelnetMonitor::deInit()
{
	return true;
}

bool TelnetMonitor::timeSlice()
{
	for( int j = 0; j < lCon.getSize(); j++ )
	{
		if( ((Connection *)lCon[j])->hasInput() )
		{
			printf("%s\n", ((Connection *)lCon[j])->getInput() );
		}
	}
	return true;
}

LinkMessage* TelnetMonitor::processIRM( LinkMessage *pMsg )
{
}
	
bool TelnetMonitor::onNewConnection( Connection *pCon, int nPort )
{
	ProtocolTelnet *pt = new ProtocolTelnet();
	pCon->setProtocol( pt );

	lCon.append( pt );
	
	return true;
}

bool TelnetMonitor::onClosedConnection( Connection *pCon )
{
	return true;
}

