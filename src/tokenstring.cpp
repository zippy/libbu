/***************************************************************************
 *   Copyright (C) 2003 by Mike Buland                                     *
 *   eichlan@Xagafinelle                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "tokenstring.h"
#include <string.h>

TokenString::TokenString( const char *lpNewTokenString )
{
	lpTokenString = NULL;
	if( lpNewTokenString )
	{
		parseLine( lpNewTokenString );
	}
}

TokenString::~TokenString()
{
	delete[] lpTokenString;
	for( int j = 0; j < lToken.getSize(); j++ )
	{
		delete[] (((Token *)lToken[j])->lpToken);
		delete ((Token *)lToken[j]);
	}
}

void TokenString::parseLine( const char *lpNewTokenString )
{
	if( lpTokenString != NULL )
	{
		delete[] lpTokenString;
		lpTokenString = NULL;
		for( int j = 0; j < lToken.getSize(); j++ )
		{
			delete[] (((Token *)lToken[j])->lpToken);
			delete ((Token *)lToken[j]);
		}
		lToken.empty();
	}
	if( lpNewTokenString == NULL )
	{
		lpTokenString = new char[1];
		lpTokenString[0] = '\0';
		lToken.setSize(0);
		return;
	}
	// First order of business, make an internal copy so someone can get it
	// if they want to.
	int nLen = strlen(lpNewTokenString);
	lpTokenString = new char[nLen+1];
	strcpy( lpTokenString, lpNewTokenString );

	// Now we do a preliminary parse.  This could be effected by later
	// editing and aliasing, but we'll see...
	int nTkStart, nTkEnd;
	int mode=0;  // 0 = startSearch, 1=endSearch
	for( int j = 0; j <= nLen; j++ )
	{
		if( mode == 0 )
		{
			if( lpTokenString[j] != ' ' &&
				lpTokenString[j] != '\t' )
			{
				nTkStart = j;
				mode = 1;
			}
		}
		else
		{
			if( lpTokenString[j] == ' ' ||
				lpTokenString[j] == '\t' ||
				lpTokenString[j] == '\0' )
			{
				nTkEnd = j-1;
				mode = 0;

				appendToken( nTkStart, nTkEnd );
			}
		}
	}
}

void TokenString::appendToken( int nStart, int nEnd )
{
	Token *pToken = new Token;
	pToken->lpOrig = &lpTokenString[nStart];

	// nStart and nEnd are inclusive, we must add two for the end, and the null
	pToken->lpToken = new char[nEnd-nStart+2];
	memcpy( pToken->lpToken, &lpTokenString[nStart], nEnd-nStart+1 );
	pToken->lpToken[nEnd-nStart+1] = '\0';

//	printf("%s\n", pToken->lpToken );
	lToken.append( pToken );
}

void TokenString::insertToken( int nStart, int nEnd, char *lpOldOrig, const char *lpNewToken, int nIndex )
{
	Token *pToken = new Token;
	pToken->lpOrig = lpOldOrig;

	// nStart and nEnd are inclusive, we must add two for the end, and the null
	pToken->lpToken = new char[nEnd-nStart+2];
	memcpy( pToken->lpToken, &lpNewToken[nStart], nEnd-nStart+1 );
	pToken->lpToken[nEnd-nStart+1] = '\0';

	lToken.insertBefore( pToken, nIndex );
}

int TokenString::getNumTokens()
{
	return lToken.getSize();
}

char *TokenString::getToken( int nIndex )
{
	if( nIndex >= lToken.getSize() ) return NULL;
	return (char *)(((Token *)lToken[nIndex])->lpToken);
}

char *TokenString::getTokenString( int nIndex )
{
	if( nIndex >= lToken.getSize() ) return NULL;
	return (char *)(((Token *)lToken[nIndex])->lpOrig);
}

void TokenString::expandTokenTo( int nIndex, char *lpNewToken )
{
	// First, we delete the token at nIndex, then we keep inserting
	// at that position...
	// We also have to remember the index to the original string,
	// since most of what we're expanding to won't be in the origingal
	// we need to keep these indexes updated in order to make other parts
	// of the system happy.
	char *lpOldOrig = ((Token *)lToken[nIndex])->lpOrig;
	delete[] ((Token *)lToken[nIndex])->lpToken;
	delete ((Token *)lToken[nIndex]);
	lToken.deleteAt( nIndex );

	// We'll do this just like we did above, but instead we'll
	// do tricky things when we find tokens...
	int nLen = strlen(lpNewToken);
	int nTkStart, nTkEnd, nNewIndex=nIndex;
	int mode=0;  // 0 = startSearch, 1=endSearch
	for( int j = 0; j <= nLen; j++ )
	{
		if( mode == 0 )
		{
			if( lpNewToken[j] != ' ' && lpNewToken[j] != '\t' )
			{
				nTkStart = j;
				mode = 1;
			}
		}
		else
		{
			if( lpNewToken[j] == ' ' || lpNewToken[j] == '\t' || lpNewToken[j] == '\0' )
			{
				nTkEnd = j-1;
				mode = 0;

				insertToken( nTkStart, nTkEnd, lpOldOrig, lpNewToken, nNewIndex );
				nNewIndex++;
			}
		}
	}
}
