/***************************************************************************
                          programlink.cpp  -  description
                             -------------------
    begin                : Sat Sep 6 2003
    copyright            : (C) 2003 by Mike Buland
    email                : eichlan@yf-soft.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
