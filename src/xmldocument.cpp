#include <stdio.h>
#include <stdlib.h>
#include "xmlwriter.h"

XmlDocument::XmlDocument( XmlNode *pRoot )
{
	this->pRoot = pRoot;
	pCurrent = NULL;
	bCompleted = (pRoot!=NULL);
}

XmlDocument::~XmlDocument()
{
	if( pRoot )
	{
		delete pRoot;
	}
}

void XmlDocument::addNode( const char *sName, const char *sContent, bool bClose )
{
	if( pRoot == NULL )
	{
		// This is the first node, so ignore position and just insert it.
		pCurrent = pRoot = new XmlNode( sName, NULL, sContent );
	}
	else
	{
		pCurrent = pCurrent->addChild( sName, sContent );
	}

	if( bClose )
	{
		closeNode();
	}
}

void XmlDocument::setName( const char *sName )
{
	pCurrent->setName( sName );
}

bool XmlDocument::isCompleted()
{
	return bCompleted;
}

XmlNode *XmlDocument::getRoot()
{
	return pRoot;
}

XmlNode *XmlDocument::detatchRoot()
{
	XmlNode *pTemp = pRoot;
	pRoot = NULL;
	return pTemp;
}

XmlNode *XmlDocument::getCurrent()
{
	return pCurrent;
}

void XmlDocument::closeNode()
{
	if( pCurrent != NULL )
	{
		pCurrent = pCurrent->getParent();
		
		if( pCurrent == NULL )
		{
			bCompleted = true;
		}
	}
}

void XmlDocument::addProperty( const char *sName, const char *sValue )
{
	if( pCurrent )
	{
		pCurrent->addProperty( sName, sValue );
	}
}

void XmlDocument::addProperty( const char *sName, const unsigned char nValue )
{
	char buf[12];
	sprintf( buf, "%hhi", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const char nValue )
{
	char buf[12];
	sprintf( buf, "%hhi", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const unsigned short nValue )
{
	char buf[12];
	sprintf( buf, "%hi", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const short nValue )
{
	char buf[12];
	sprintf( buf, "%hi", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const int nValue )
{
	char buf[12];
	sprintf( buf, "%d", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const unsigned long nValue )
{
	char buf[12];
	sprintf( buf, "%li", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const long nValue )
{
	char buf[12];
	sprintf( buf, "%li", nValue );
	addProperty( sName, buf );
}

void XmlDocument::addProperty( const char *sName, const double dValue )
{
	char buf[40];
	sprintf( buf, "%f", dValue );
	addProperty( sName, buf );
}

void XmlDocument::setContent( const char *sContent )
{
	if( pCurrent )
	{
		pCurrent->setContent( sContent );
	}
}

