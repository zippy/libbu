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
