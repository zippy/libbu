#include <stdlib.h>
#include "programchain.h"

ProgramChain::ProgramChain() :
	xLog( MultiLog::getInstance() )
{
	xLog.LineLog( MultiLog::LStatus, "Program Chain Initialized." );
}

ProgramChain::~ProgramChain()
{
}

bool ProgramChain::addLink( ProgramLink *pLink )
{
	if( pLink->init() == false )
	{
		emergencyShutdown();
		return false;
	}

	lLink.append( pLink );

	pLink->setChain( this );

	return true;
}

ProgramLink *ProgramChain::getLink( const char *lpName )
{
	char a;
	a = lpName[0];
	return NULL;
}

ProgramLink *ProgramChain::getBaseLink()
{
	return NULL;
}

bool ProgramChain::execChainOnce()
{
	int nLen = lLink.getSize();
	for( int j = 0; j < nLen; j++ )
	{
		if( ((ProgramLink *)lLink[j])->timeSlice() == false )
		{
			xLog.LineLog( MultiLog::LInfo, "Shutting down due to signal from link #%d", j );
			emergencyShutdown();
			return false;
		}
	}

	return true;
}

bool ProgramChain::enterChainLoop()
{
	for(;;)
	{
		if( execChainOnce() == false )
		{
			return false;
		}
	}

	return true;
}

void ProgramChain::emergencyShutdown()
{
	int nLen = lLink.getSize();
	for( int j = 0; j < nLen; j++ )
	{
		((ProgramLink *)lLink[j])->deInit();
		delete (ProgramLink *)lLink[j];
	}
	lLink.empty();
}

LinkMessage *ProgramChain::broadcastIRM( LinkMessage *pMsgOut, ProgramLink *pSender )
{
	int nLen = lLink.getSize();
	for( int j = 0; j < nLen; j++ )
	{
		LinkMessage *pMsg = ((ProgramLink *)lLink[j])->processIRM( pMsgOut );
		if( pMsg != NULL )
		{
			delete pMsgOut;
			return pMsg;
		}
	}

	delete pMsgOut;
	return NULL;
}
