#include <stdio.h>
#include <stdlib.h>
#include "xmlwriter.h"

XmlWriter::XmlWriter( const Bu::FString &sIndent, XmlNode *pRoot ) :
	XmlDocument( pRoot ),
	sIndent( sIndent )
{
}

XmlWriter::~XmlWriter()
{
}

void XmlWriter::write()
{
	write( getRoot(), sIndent.c_str() );
}

void XmlWriter::write( XmlNode *pRoot, const Bu::FString &sIndent )
{
	writeNode( pRoot, 0, sIndent );
}

void XmlWriter::closeNode()
{
	XmlDocument::closeNode();

	if( isCompleted() )
	{
		write( getRoot(), sIndent.c_str() );
	}
}

void XmlWriter::writeIndent( int nIndent, const Bu::FString &sIndent )
{
	if( sIndent == NULL ) return;
	for( int j = 0; j < nIndent; j++ )
	{
		writeString( sIndent );
	}
}

Bu::FString XmlWriter::escape( const Bu::FString &sIn )
{
	Bu::FString sOut;

	int nMax = sIn.getSize();
	for( int j = 0; j < nMax; j++ )
	{
		char c = sIn[j];
		if( ((c >= ' ' && c <= '9') ||
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ) &&
			(c != '\"' && c != '\'' && c != '&')
		  )
		{
			sOut += c;
		}
		else
		{
			sOut += "&#";
			char buf[4];
			sprintf( buf, "%u", (unsigned char)c );
			sOut += buf;
			sOut += ';';
		}
	}

	return sOut;
}

void XmlWriter::writeNodeProps( XmlNode *pNode, int nIndent, const Bu::FString &sIndent )
{
	for( int j = 0; j < pNode->getNumProperties(); j++ )
	{
		writeString(" ");
		//writeString( pNode->getPropertyName( j ) );
		writeString("=\"");
		//writeString( escape( pNode->getProperty( j ) ).c_str() );
		writeString("\"");
	}
}

void XmlWriter::writeNode( XmlNode *pNode, int nIndent, const Bu::FString &sIndent )
{
	if( pNode->hasChildren() )
	{
		writeIndent( nIndent, sIndent );
		writeString("<");
		writeString( pNode->getName() );
		writeNodeProps( pNode, nIndent, sIndent );
		if( sIndent != "" )
			writeString(">\n");
		else
			writeString(">");
/*
		if( pNode->getContent( 0 ) )
		{
			writeIndent( nIndent+1, sIndent );
			if( sIndent != "" )
			{
				writeString( pNode->getContent( 0 ) );
				writeString("\n");
			}
			else
				writeString( pNode->getContent( 0 ) );
		}

		int nNumChildren = pNode->getNumChildren();
		for( int j = 0; j < nNumChildren; j++ )
		{
			writeNode( pNode->getChild( j ), nIndent+1, sIndent );
			if( pNode->getContent( j+1 ) )
			{
				writeIndent( nIndent+1, sIndent );
				if( sIndent )
				{
					writeString( pNode->getContent( j+1 ) );
					writeString("\n");
				}
				else
					writeString( pNode->getContent( j+1 ) );
			}
		}
*/		
		writeIndent( nIndent, sIndent );
		if( sIndent != "" )
		{
			writeString("</");
			writeString( pNode->getName() );
			writeString(">\n");
		}
		else
		{
			writeString("</");
			writeString( pNode->getName() );
			writeString(">");
		}
	}/*
	else if( pNode->getContent() )
	{
		writeIndent( nIndent, sIndent );
		writeString("<");
		writeString( pNode->getName() );
		writeNodeProps( pNode, nIndent, sIndent );
		writeString(">");
		writeString( pNode->getContent() );
		writeString("</");
		writeString( pNode->getName() );
		writeString(">");
		if( sIndent )
			writeString("\n");
	}*/
	else
	{
		writeIndent( nIndent, sIndent );
		writeString("<");
		writeString( pNode->getName() );
		writeNodeProps( pNode, nIndent, sIndent );
		if( sIndent != "" )
			writeString("/>\n");
		else
			writeString("/>");
	}
}

