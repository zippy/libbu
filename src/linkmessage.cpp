/***************************************************************************
 *   Copyright (C) 2003 by Mike Buland                                     *
 *   eichlan@yf-soft.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "linkmessage.h"
#include <string.h>

LinkMessage::LinkMessage( int nNewMsg )
{
	nMsg = nNewMsg;
}

LinkMessage::~LinkMessage()
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
