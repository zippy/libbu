#include <stdio.h>
#include <stdlib.h>
#include "xmlstringwriter.h"

XmlStringWriter::XmlStringWriter( const char *sIndent ) :
	XmlWriter( sIndent )
{
}

XmlStringWriter::~XmlStringWriter()
{
}

void XmlStringWriter::writeString( const char *sString )
{
	sXml += sString;
}

std::string &XmlStringWriter::getString()
{
	return sXml;
}

