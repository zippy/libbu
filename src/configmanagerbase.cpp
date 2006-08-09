#include <stdlib.h>
#include <string.h>
#include "xmlnode.h"
#include "xmlfilereader.h"
#include "configmanagerbase.h"

ConfigManagerBase::ConfigManagerBase()
{
}

ConfigManagerBase::~ConfigManagerBase()
{
}

void ConfigManagerBase::addSearchPath( const std::string &sPath )
{
	lSearchPath.push_back( sPath );
}

void ConfigManagerBase::loadConfig( const std::string &sFileName, const char *lpProfile )
{
	// Try a few locations...
	std::list<std::string>::const_iterator i;
	for( i = lSearchPath.begin(); i != lSearchPath.end(); i++ )
	{
		if( parseConfig( (*i+sFileName).c_str(), lpProfile ) )
		{
			break;
		}
	}
}

bool ConfigManagerBase::parseConfig( const char *lpFileName, const char *lpProfile )
{
	XmlNode *pRoot, *pCur;
	XmlFileReader doc( lpFileName );

	pRoot = doc.getRoot();
	if( pRoot == NULL )
	{
		return false;
	}

	if( strcmp("config", pRoot->getName() ) )
	{
		return false;
	}

	for( int j = 0;; j++ )
	{
		pCur = pRoot->getChild( "profile", j );
		if( pCur == NULL )
			return false;

		if( !strcmp( pCur->getProperty("id"), lpProfile ) )
		{
			return processProfile( pCur );
		}
	}

	return false;
}

