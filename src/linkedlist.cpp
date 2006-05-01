/***************************************************************************
                          linkedlist.cpp  -  description
                             -------------------
    begin                : Sun Oct 19 2003
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

#include "linkedlist.h"

LinkedList::LinkedList(  )
{
    pBase = NULL;
	pTop = NULL;
    pLast = NULL;
    nSize = 0;
    nLast = -1;
}

LinkedList::~LinkedList(  )
{
/*
    Link *pCur = pBase;
    while( pCur )
    {
        Link *pLast = pCur;
        pCur = pCur->pNext;
        delete pLast;
    }
*/
	empty();
}

void *LinkedList::getAt( int index )
{
    if( index < 0 || index >= nSize )
        return NULL;

	return getPtrTo( index )->pData;
}

void LinkedList::append( void *data )
{
    if( pBase == NULL )
    {
        pBase = new Link( data );
		pTop = pBase;
        nSize++;
    }
    else
    {
		pTop->pNext = new Link( data );
		pTop = pTop->pNext;
        nSize++;
    }
}

void LinkedList::insertBefore( void *data, int pos )
{
    if( pos < 0 || pos > nSize )
        return;

    if( pos == 0 )
    {
        Link *pTmp = new Link( data, pBase );
		if( pBase == NULL )
		{
			pTop = pTmp;
		}
        pBase = pTmp;
		if( nLast >= 0 ) nLast++;
        nSize++;
    }
    else
    {
		Link *pCur;
		if( (pCur = getPtrTo( pos-1 )) == NULL )
		{
			return;
		}
        Link *pNew = new Link( data, pCur->pNext );
        pCur->pNext = pNew;
		if( pNew->pNext == NULL )
		{
			pTop = pNew;
		}
		if( nLast >= pos ) nLast++;
        nSize++;
    }
}

int LinkedList::getSize(  )
{
    return nSize;
}

bool LinkedList::isEmpty(  )
{
    if( nSize == 0 )
        return true;
    return false;
}

void LinkedList::deleteAt( int index )
{
	if( index >= nSize ||
		pBase == NULL )
		return;

	if( index == 0 )
	{
		Link *pTmp = pBase->pNext;
		delete pBase;
		pBase = pTmp;
		if( nLast >= 0 ) nLast--;
		nSize--;
		if( pBase == NULL )
		{
			pTop = NULL;
		}
		else if( pBase->pNext == NULL )
		{
			pTop = pBase;
		}
	}
	else
	{
		Link *pCur = getPtrTo( index-1 );
		if( pCur->pNext == pTop )
		{
			pTop = pCur;
		}
		Link *pTmp;
		if( pCur->pNext == NULL )
		{
			pTmp = NULL;
		}
		else
		{
			pTmp = pCur->pNext->pNext;
		}
		delete pCur->pNext;
		pCur->pNext = pTmp;
		if( nLast == index ) nLast = -1;
		else if( index < nLast ) nLast--;
		nSize--;
	}
}

void LinkedList::empty()
{
	while( nSize > 0 )
	{
		deleteAt( 0 );
	}
}

void LinkedList::setSize( int newSize )
{
	if( newSize < nSize )
	{
		// Delete items off of the end of the list.
		while( nSize > newSize )
		{
			deleteAt( nSize-1 );
		}
	}
	else
	{
		// Add null items to the end of the list.
		while( nSize < newSize )
		{
			append( NULL );
		}
	}
}

void LinkedList::setAt( int index, void *data )
{
	if( index >= nSize || index < 0 )
		return;

	getPtrTo( index )->pData = data;
}

LinkedList::Link *LinkedList::getPtrTo( int index )
{
    if( index < 0 || index >= nSize )
        return NULL;
    if( index == nLast )
    {
        return pLast;
    }
    if( index == 0 )
    {
        pLast = pBase;
        nLast = 0;
        return pBase;
    }
    else
    {
        Link *pCur = pBase;
        int nCur = 0;
        if( nLast < index && nLast >= 0 )
        {
            pCur = pLast;
            nCur = nLast;
        }
        while( nCur != index )
        {
            pCur = pCur->pNext;
            nCur++;
        }
        nLast = index;
        pLast = pCur;
        return pCur;
    }
}
