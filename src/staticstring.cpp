#include "staticstring.h"
#include "serializer.h"

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

StaticString::StaticString( const char *lpNewStr )
{
	lpStr = NULL;
	nLen = 0;
	setString( lpNewStr, -1 );
}

StaticString::StaticString( StaticString &xSrcStr, int nNewLen )
{
	lpStr = NULL;
	nLen = 0;
	setString( xSrcStr, nNewLen );
}

StaticString::StaticString( StaticString &xSrcStr )
{
	lpStr = NULL;
	nLen = 0;
	setString( xSrcStr, -1 );
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

void StaticString::setLength( int nNewLength )
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

char StaticString::getAt( unsigned int nIndex )
{
	if( nIndex < 0 || nIndex >= nLen )
		return '\0';

	return lpStr[nIndex];
}

void StaticString::setAt( unsigned int nIndex, char cVal )
{
	if( nIndex < 0 || nIndex >= nLen )
		return;

	lpStr[nIndex] = cVal;
}

char &StaticString::operator[]( unsigned int nIndex )
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

void StaticString::serialize( Serializer &ar )
{
	if( ar.isLoading() )
	{
		ar >> nLen;
		setLength( nLen );
		ar.read( lpStr, nLen );
	}
	else
	{
		ar << nLen;
		ar.write( lpStr, nLen );
	}
}

bool StaticString::operator==( const char *str )
{
	const char *a = str, *b = lpStr;
	for(; *a == *b; a++, b++ ) if( *a == '\0' && *b == '\0' ) return true;
	return false;
}

bool StaticString::operator==( StaticString &str )
{
	const char *a = str.lpStr, *b = lpStr;
	for(; *a == *b; a++, b++ ) if( *a == '\0' && *b == '\0' ) return true;
	return false;
}

bool StaticString::operator!=( const char *str )
{
	const char *a = str, *b = lpStr;
	for(; *a == *b; a++, b++ ) if( *a == '\0' && *b == '\0' ) return false;
	return true;
}

bool StaticString::operator!=( StaticString &str )
{
	const char *a = str.lpStr, *b = lpStr;
	for(; *a == *b; a++, b++ ) if( *a == '\0' && *b == '\0' ) return false;
	return true;
}
/*
unsigned long int StaticString::getHashCode()
{
	unsigned long int nPos = nLen;
	unsigned long int j = 0;
	for( const char *s = (const char *)lpStr; j< nLen; s++, j++ )
	{
		nPos = *s + (nPos << 6) + (nPos << 16) - nPos;
	}
	return nPos;
}

bool StaticString::compareForHash( Hashable &other )
{
	if( ((StaticString &)other).nLen != nLen )
		return false;

	const char *a = ((StaticString &)other).lpStr;
	const char *b = lpStr;
	if( a == b )
		return true;

	for( unsigned long j = 0; j < nLen; j++, a++, b++ )
		if( *a != *b )
			return false;

	return true;
}
*/
