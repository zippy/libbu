#ifndef LINK_MESSENGER_H
#define LINK_MESSENGER_H

#include <stdlib.h>
#include <stdint.h>
#include "linkmessage.h"

class LinkMessenger
{
public:
	LinkMessenger();
	virtual ~LinkMessenger();

	void enqueueMessage( LinkMessage *pMsg );
	LinkMessage *dequeueMessage();
	bool hasMessages()
	{
		return (pFirst != NULL);
	}

private:
	typedef struct Link
	{
		LinkMessage *pMsg;
		Link *pNext;
	};
	Link *pFirst;
	Link *pLast;

};

#endif
