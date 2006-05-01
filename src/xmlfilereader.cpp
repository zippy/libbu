#include "xmlfilereader.h"
#include <string.h>

XmlFileReader::XmlFileReader( const char *sFile, bool bStrip )
	: XmlReader( bStrip )
{
	fh = fopen( sFile, "rt" );

	if( fh == NULL )
	{
		reportError("Couldn't open file.");
		//nError = 1;
	}
	else
	{
		char buf[50];
		fgets( buf, 50, fh );

		if( !strcmp( buf, "<?xml version=\"1.0\"?>\n" ) )
		{
			buildDoc();
		}
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
		return '\0';
	}
}

void XmlFileReader::usedChar()
{
	if( fbDataIn.getLength() > 0 )
	{
		fbDataIn.usedData( 1 );
	}
}
