/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdlib.h>
#include "bu/programchain.h"
#include "bu/programlink.h"

using namespace Bu;

Bu::ProgramChain::ProgramChain()
{
}

Bu::ProgramChain::~ProgramChain()
{
}

bool Bu::ProgramChain::addLink( ProgramLink *pLink )
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

ProgramLink *Bu::ProgramChain::getLink( const char *lpName )
{
	char a;
	a = lpName[0];
	return NULL;
}

ProgramLink *Bu::ProgramChain::getBaseLink()
{
	return NULL;
}

bool Bu::ProgramChain::execChainOnce()
{
	for( Bu::List<Bu::ProgramLink *>::iterator i = lLink.begin();
		 i != lLink.end(); i++ )
	{
		if( (*i)->timeSlice() == false )
		{
			emergencyShutdown();
			return false;
		}
	}

	return true;
}

bool Bu::ProgramChain::enterChainLoop()
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

void Bu::ProgramChain::emergencyShutdown()
{
	for( Bu::List<Bu::ProgramLink *>::iterator i = lLink.begin();
		 i != lLink.end(); i++ )
	{
		(*i)->deInit();
		delete *i;
	}
	lLink.clear();
}

LinkMessage *Bu::ProgramChain::broadcastIRM( LinkMessage *pMsgOut, ProgramLink *pSender )
{
	for( Bu::List<Bu::ProgramLink *>::iterator i = lLink.begin();
		 i != lLink.end(); i++ )
	{
		LinkMessage *pMsg = (*i)->processIRM( pMsgOut );
		if( pMsg != NULL )
		{
			delete pMsgOut;
			return pMsg;
		}
	}

	delete pMsgOut;
	return NULL;
}

