#include "programlink.h"
#include "programchain.h"

ProgramLink::ProgramLink()
{
}

ProgramLink::~ProgramLink()
{
}

LinkMessage *ProgramLink::sendIRM( LinkMessage *pMsgOut )
{
	return pChain->broadcastIRM( pMsgOut, this );
}

void ProgramLink::setChain( ProgramChain *pNewChain )
{
	pChain = pNewChain;
}

/*
void ProgramLink::postMessage( LinkMessage *pMsg, int nLvl )
{
	if( nLvl == msgToChain )
	{
		qMsgToChain.enqueue( pMsg );
	}
	else if( nLvl == msgToLink )
	{
		qMsgToLink.enqueue( pMsg );
	}
	else
	{
		// ERROR!
	}
}

LinkMessage *ProgramLink::getMessage( int nLvl )
{
	if( nLvl == msgToChain )
	{
		return (LinkMessage *)qMsgToChain.dequeue();
	}
	else if( nLvl == msgToLink )
	{
		return (LinkMessage *)qMsgToLink.dequeue();
	}
	else
	{
		// ERROR!
	}
}
*/
