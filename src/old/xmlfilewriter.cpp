#include <stdio.h>
#include <stdlib.h>
#include "xmlfilewriter.h"

XmlFileWriter::XmlFileWriter( const char *sFileName, const char *sIndent, XmlNode *pRoot ) :
	XmlWriter( sIndent, pRoot )
{
	this->sFileName = sFileName;
	fh = fopen( sFileName, "wt");
	fprintf( fh, "<?xml version=\"1.0\"?>\n");
}

XmlFileWriter::XmlFileWriter( FILE *fh, const char *sIndent, XmlNode *pRoot ) :
	XmlWriter( sIndent, pRoot ),
	fh( fh )
{
}

XmlFileWriter::~XmlFileWriter()
{
	fclose( fh );
}

void XmlFileWriter::writeString( const char *sString )
{
	fputs( sString, fh );
}

