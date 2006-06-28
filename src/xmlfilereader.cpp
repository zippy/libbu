#include "xmlfilereader.h"
#include "xmlexception.h"
#include <string.h>

XmlFileReader::XmlFileReader( const char *sFile, bool bStrip )
	: XmlReader( bStrip )
{
	fh = fopen( sFile, "rt" );

	if( fh == NULL )
	{
		throw XmlException("Couldn't open file: %s", sFile );
		//nError = 1;
	}
	else
	{
		buildDoc();
	}
}

XmlFileReader::~XmlFileReader()
{
}

char XmlFileReader::getChar( int nIndex )
{
	// Make sure we always have a little data left in the buffer
	if( fbDataIn.getLength() <= nIndex+1 && fh )
	{
		int nBytes = fbDataIn.getCapacity()-1;
		char *buf = new char[nBytes];
		int nRead = fread( buf, 1, nBytes, fh );
		fbDataIn.appendData( buf, nRead );
		delete[] buf;

		if( nRead < nBytes )
		{
			fclose( fh );
			fh = NULL;
		}
	}
	if( fbDataIn.getLength() >= nIndex+1 )
	{
		return fbDataIn.getData()[nIndex];
	}
	else
	{
		throw XmlException("End of XML stream read.");
	}
}

void XmlFileReader::usedChar( int nAmnt )
{
	if( fbDataIn.getLength()-nAmnt >= 0 )
	{
		fbDataIn.usedData( nAmnt );
	}
}
