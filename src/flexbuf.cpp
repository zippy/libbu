#include "flexbuf.h"
#include <string.h>

FlexBuf::FlexBuf()
{
	lpBuf = new char[1024];
	nLastChar = 0;
	nFirstChar = 0;
	nSize = 1024;
	nFill = 0;
	clearData();
}

FlexBuf::~FlexBuf()
{
	delete[] lpBuf;
}

bool FlexBuf::appendData( const char *lpData, int nDSize )
{
	int nStrLen;
	if( nDSize < 0 )
	{
		nStrLen = strlen( lpData );
	}
	else
	{
		nStrLen = nDSize;
	}

	if( nLastChar + nStrLen + 1 > nSize )
	{
		if( nFill + nStrLen + 1 < nSize )
		{
			memcpy( lpBuf, lpBuf+nFirstChar, nFill );
			nLastChar -= nFirstChar;
			nFirstChar = 0;
		}
		else
		{
			nSize += nStrLen+1;
			char *lpNewBuf = new char[nSize];
			memcpy( lpNewBuf, lpBuf+nFirstChar, nFill );
			delete[] lpBuf;
			lpBuf = lpNewBuf;
			nLastChar -= nFirstChar;
			nFirstChar = 0;
		}
	}

	memcpy( &lpBuf[nLastChar], lpData, nStrLen );
	nLastChar += nStrLen;
	nFill += nStrLen;
	lpBuf[nLastChar] = '\0';

	return true;
}

bool FlexBuf::appendData( const char lData )
{
	if( nLastChar + 2 > nSize )
	{
		if( nFill+2 < nSize )
		{
			memcpy( lpBuf, lpBuf+nFirstChar, nFill );
			nLastChar -= nFirstChar;
			nFirstChar = 0;
		}
		else
		{
			nSize += 1024;
			char *lpNewBuf = new char[nSize];
			memcpy( lpNewBuf, lpBuf+nFirstChar, nFill );
			delete[] lpBuf;
			lpBuf = lpNewBuf;
			nLastChar -= nFirstChar;
			nFirstChar = 0;
		}
	}

	lpBuf[nLastChar] = lData;
	nLastChar++;
	nFill++;
	lpBuf[nLastChar] = '\0';

	return true;
}

bool FlexBuf::appendData( const short lData )
{
	return appendData( (const char *)&lData, sizeof(short) );
}

bool FlexBuf::appendData( const int lData )
{
	return appendData( (const char *)&lData, sizeof(int) );
}

bool FlexBuf::appendData( const long lData )
{
	return appendData( (const char *)&lData, sizeof(long) );
}

bool FlexBuf::appendData( const float lData )
{
	return appendData( (const char *)&lData, sizeof(float) );
}

bool FlexBuf::appendData( const double lData )
{
	return appendData( (const char *)&lData, sizeof(double) );
}

bool FlexBuf::appendData( const unsigned char lData )
{
	return appendData( (const char)lData );
}

bool FlexBuf::appendData( const unsigned short lData )
{
	return appendData( (const char *)&lData, sizeof(short) );
}

bool FlexBuf::appendData( const unsigned long lData )
{
	return appendData( (const char *)&lData, sizeof(long) );
}

bool FlexBuf::appendData( const unsigned int lData )
{
	return appendData( (const char *)&lData, sizeof(int) );
}

bool FlexBuf::clearData()
{
	nFirstChar = nLastChar = nFill = 0;
	lpBuf[nLastChar] = '\0';

	return true;
}

const char *FlexBuf::getData()
{
	return (lpBuf+nFirstChar);
}

int FlexBuf::getLength()
{
	return nFill;
}

int FlexBuf::getCapacity()
{
	return nSize;
}

bool FlexBuf::usedData( int nAmount )
{
	// Remove from the end if negative
	if( nAmount < 0 )
	{
		if( nFill+nAmount < 0 )
		{
			nFill = nFirstChar = nLastChar = 0;
			return true;
		}
		nLastChar += nAmount;
		nFill += nAmount;
		return true;
	}
	if( nAmount > nFill )
	{
		nAmount = nSize;
//		return false;
	}

	//nLastChar -= nAmount;
	nFirstChar += nAmount;
	nFill -= nAmount;
	
	if( nFill == 0 )
	{
		nFirstChar = nLastChar = 0;
	}

	//if( nLastChar > 0 )
	//{
		//memmove( lpBuf, &lpBuf[nAmount], nLastChar );
	//}

	return true;
}

int FlexBuf::findChar( char cTarget )
{
	for( int j = nFirstChar; j < nLastChar; j++ )
	{
		if( lpBuf[j] == cTarget )
		{
			return j;
		}
	}

	return -1;
}

