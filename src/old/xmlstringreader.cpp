#include "xmlstringreader.h"
#include "exceptions.h"
#include <string.h>

XmlStringReader::XmlStringReader( const char *sString, bool bStrip )
	: XmlReader( bStrip )
{
	this->sString = sString;
	
	nIndex = 0;
	nLength = strlen( sString );

	buildDoc();
}

XmlStringReader::~XmlStringReader()
{
}

char XmlStringReader::getChar( int nAdd )
{
	if( nLength >= nIndex+nAdd+1 )
	{
		return sString[nIndex+nAdd];
	}
	else
	{
		throw XmlException("End of XML stream read.");
	}
}

void XmlStringReader::usedChar( int nAmnt )
{
	if( nLength >= nIndex+nAmnt )
	{
		nIndex += nAmnt;
	}
}
