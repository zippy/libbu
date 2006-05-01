//
// C++ Implementation: ringlist
//
// Description: 
//
//
// Author: Mike Buland <eichlan@yf-soft.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <stdlib.h>

#include "ringlist.h"

RingList::RingList( int nInitSize )
 : List()
{
	nFirstIndex = 0;
	nRealLength = nInitSize;
	nDataLength = 0;
	apData = new void*[nInitSize];
	pPushBuf = NULL;
}

RingList::~RingList()
{
	delete[] apData;
}

void *RingList::getAt( int nIndex )
{
	if( nIndex < 0 || nIndex >= nDataLength )
	{
		return NULL;
	}
	
	return apData[(nFirstIndex+nIndex)%nRealLength];
}

void RingList::append( void *pData )
{
	int nIndex = (nFirstIndex+nDataLength)%nRealLength;
	
	pPushBuf = apData[nIndex];
	apData[nIndex] = pData;
	
	if( nDataLength == nRealLength )
	{
		nFirstIndex = (nFirstIndex+1)%nRealLength;
	}
	else
	{
		nDataLength++;
		// We really didn't need it this time...
		pPushBuf = NULL;
	}
}

void RingList::insertBefore( void *pData, int nPos )
{
	// Not implemented right now, don't even try it!
}

int RingList::getSize()
{
	return nDataLength;
}

bool RingList::isEmpty()
{
	return nDataLength==0;
}

void RingList::deleteAt( int nIndex )
{
	// Also not implemented yet
}

void RingList::empty()
{
	nFirstIndex = 0;
	nDataLength = 0;
}

void RingList::setSize( int nNewSize )
{
	if( apData )
	{
		delete[] apData;
	}
	nFirstIndex = 0;
	nRealLength = nNewSize;
	nDataLength = 0;
	apData = new void*[nNewSize];
}

void RingList::setAt( int nIndex, void *pData )
{
	apData[(nIndex+nFirstIndex)%nRealLength] = pData;
}

void *RingList::getPushBuf()
{
	return pPushBuf;
}
