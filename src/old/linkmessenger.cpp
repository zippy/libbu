#include "linkmessenger.h"

LinkMessenger::LinkMessenger() :
	pFirst( NULL ),
	pLast( NULL )
{
}

LinkMessenger::~LinkMessenger()
{
}

void LinkMessenger::enqueueMessage( LinkMessage *pMsg )
{
	if( pLast == NULL )
	{
		pFirst = pLast = new Link;
		pLast->pMsg = pMsg;
		pLast->pNext = NULL;
	}
	else
	{
		pLast->pNext = new Link;
		pLast = pLast->pNext;
		pLast->pMsg = pMsg;
		pLast->pNext = NULL;
	}
}

LinkMessage *LinkMessenger::dequeueMessage()
{
	if( pFirst == NULL )
		return NULL;

	Link *pTmp = pFirst;
	pFirst = pFirst->pNext;
	LinkMessage *pRet = pTmp->pMsg;
	delete pTmp;
	return pRet;
}

