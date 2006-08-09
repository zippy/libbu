#include <stdlib.h>
#include <string.h>
#include "xmlnode.h"
#include "xmlfilereader.h"
#include "configmanager.h"
#include "multilog.h"
#include "multilogtext.h"
#include "config.h"

#ifndef DEF_DEF_LANG
# error You must define a default default language in DEF_DEF_LANG
#endif

ConfigManager::ConfigManager() :
	sDefLang( DEF_DEF_LANG )
{
}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::loadConfig( const char *lpProfile )
{
	// Try a few locations...
	char *locs[] = {"./" CONFIGNAME, SYSCONFIG, NULL};
	
	for( int j = 0; locs[j] != NULL; j++ )
	{
		if( parseConfig( locs[j], lpProfile ) )
		{
			break;
		}
	}
}

bool ConfigManager::parseConfig( const char *lpFileName, const char *lpProfile )
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

bool ConfigManager::processProfile( XmlNode *pBase )
{
	XmlNode *pCur;
	
	for( int j = 0; (pCur = pBase->getChild("listen", j)); j++ )
	{
	}
	
	for( int j = 0; (pCur = pBase->getChild("log", j)); j++ )
	{
		if( !strcmp( pCur->getProperty("type"), "text" ) )
		{
			int nLevel = atoi( pCur->getProperty("level") );
			int nArchive = atoi( pCur->getProperty("archive") );

			printf("Adding log file: %s\n", pCur->getProperty("file") );
			
			MultiLog::getInstance().addChannel(
				new MultiLogText(
					pCur->getProperty("file"),
					pCur->getProperty("format")
					)
			);
		}
	}

	XmlNode *pDefs;
	if( pDefs = pBase->getChild("defaults") )
	{
		if( pCur = pDefs->getChild("language") )
		{
			sDefLang = pCur->getContent();
		}
	}
	
	if( pCur = pBase->getChild("backend") )
	{
		sBackend = pCur->getProperty("plugin");
		sBackend = sBackend;
		pBackend = pBase->getCopy();
	}

	return true;
}

std::string &ConfigManager::getDefLanguage()
{
	return sDefLang;
}

std::string &ConfigManager::getBackend()
{
	return sBackend;
}
