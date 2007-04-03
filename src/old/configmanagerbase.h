#ifndef CONFIG_MANAGER_BASE_H
#define CONFIG_MANAGER_BASE_H

#include <string>
#include <list>

class ConfigManagerBase
{
public:
	ConfigManagerBase();
	virtual ~ConfigManagerBase();
	
public:
	void addSearchPath( const std::string &sPath );
	void loadConfig( const std::string &sFileName, const char *lpProfile="default" );

private:
	bool parseConfig( const char *lpFileName, const char *lpProfile );
	virtual bool processProfile( class XmlNode *pBase )=0;

	std::list<std::string> lSearchPath;
};

#endif
