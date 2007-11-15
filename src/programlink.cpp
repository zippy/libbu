/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/programlink.h"
#include "bu/programchain.h"

using namespace Bu;

Bu::ProgramLink::ProgramLink()
{
}

Bu::ProgramLink::~ProgramLink()
{
}

LinkMessage *Bu::ProgramLink::sendIRM( LinkMessage *pMsgOut )
{
	return pChain->broadcastIRM( pMsgOut, this );
}

void Bu::ProgramLink::setChain( ProgramChain *pNewChain )
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
