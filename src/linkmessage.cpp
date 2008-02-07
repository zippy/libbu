/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/linkmessage.h"

Bu::LinkMessage::LinkMessage( int nNewMsg )
{
	nMsg = nNewMsg;
}

Bu::LinkMessage::~LinkMessage()
{
}

/*
void LinkMessage::setBroadcast( bool bOn )
{
	bBroadcast = bOn;
}

bool LinkMessage::isBroadcast()
{
	return bBroadcast;
}


void LinkMessage::setFromID( int id )
{
	nFromLinkID = id;
}

int LinkMessage::getFromID()
{
	return nFromLinkID;
}

void LinkMessage::setToID( int id )
{
	nTargetLinkID = id;
}

int LinkMessage::getToID()
{
	return nTargetLinkID;
}
*/
