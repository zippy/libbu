#include "staticstring.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

StaticString::StaticString()
{
	lpStr = NULL;
	nLen = 0;
}

StaticString::StaticString( int nLength )
{
	lpStr = new char[nLength+1];
	nLen = nLength;
	memset( lpStr, 0, nLength+1 );
}

StaticString::StaticString( const char *lpNewStr, int nNewLen )
{
	lpStr = NULL;
	nLen = 0;
	setString( lpNewStr, nNewLen );
}

StaticString::StaticString( StaticString &xSrcStr, int nNewLen )
{
	lpStr = NULL;
	nLen = 0;
	setString( xSrcStr, nNewLen );
}

StaticString::~StaticString()
{
	if( lpStr != NULL ) delete[] lpStr;
}

char *StaticString::getString()
{
	return lpStr;
}

int StaticString::getLength()
{
	return nLen;
}

int StaticString::setLength( int nNewLength )
{
	char *lpNewStr = new char[nNewLength+1];
	if( lpStr != NULL )
	{
		strncpy( lpNewStr, lpStr, nNewLength );
	}
	lpNewStr[nNewLength] = '\0';
	if( lpStr )
	{
		delete[] lpStr;
	}
	lpStr = lpNewStr;
	nLen = nNewLength;
}

void StaticString::setString( const char *lpNewStr, int nNewLen )
{
	if( lpStr )
	{
		delete[] lpStr;
		lpStr = NULL;
		nLen = 0;
	}
	if( nNewLen < 0 )
	{
		if( lpNewStr == NULL ) return;
		nLen = strlen( lpNewStr );
		lpStr = new char[nLen+1];
		strcpy( lpStr, lpNewStr );
	}
	else
	{
		nLen = nNewLen;
		lpStr = new char[nLen+1];
		memset( lpStr, 0, nLen+1 );
		if( lpNewStr )
			strncpy( lpStr, lpNewStr, nNewLen );
	}
}

void StaticString::setString( StaticString &sNewStr, int nNewLen )
{
	if( lpStr )
	{
		delete[] lpStr;
		lpStr = NULL;
		nLen = 0;
	}
	if( nNewLen < 0 )
	{
		if( sNewStr.lpStr == NULL ) return;
		nLen = sNewStr.nLen;
		lpStr = new char[nLen+1];
		strcpy( lpStr, sNewStr.lpStr );
	}
	else
	{
		nLen = nNewLen;
		lpStr = new char[nLen+1];
		memset( lpStr, 0, nLen+1 );
		if( sNewStr.lpStr )
			strncpy( lpStr, sNewStr.lpStr, nNewLen );
	}
}

StaticString &StaticString::operator=( StaticString &lpOtherStr )
{
	setString( lpOtherStr );

	return *this;
}

StaticString &StaticString::operator=( std::string &lpOtherStr )
{
	setString( lpOtherStr.c_str() );

	return *this;
}

StaticString &StaticString::operator=( const char *lpNewStr )
{
	setString( lpNewStr );

	return *this;
}

StaticString::operator const char *()
{
	return lpStr;
}

char StaticString::getAt( int nIndex )
{
	if( nIndex < 0 || nIndex >= nLen )
		return '\0';

	return lpStr[nIndex];
}

void StaticString::setAt( int nIndex, char cVal )
{
	if( nIndex < 0 || nIndex >= nLen )
		return;

	lpStr[nIndex] = cVal;
}

char &StaticString::operator[]( int nIndex )
{
	if( nIndex < 0 || nIndex >= nLen )
		return lpStr[0];

	return lpStr[nIndex];
}

StaticString::operator int()
{
	return nLen;
}

char *StaticString::operator+( int nAmnt )
{
	return lpStr + nAmnt;
}

char *StaticString::operator-( int nAmnt )
{
	return lpStr - nAmnt;
}

void StaticString::clear()
{
	memset( lpStr, 0, nLen+1 );
}

