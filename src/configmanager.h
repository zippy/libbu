#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "config.h"
#include "singleton.h"
#include <string>

class ConfigManager : public Singleton<ConfigManager>
{
	friend class Singleton<ConfigManager>;
protected:
	ConfigManager();
	~ConfigManager();
	
public:
	void loadConfig( const char *lpProfile="default" );

private:
	bool parseConfig( const char *lpFileName, const char *lpProfile );
	bool processProfile( class XmlNode *pBase );

public:  // Getters, these should be moved if we make this a base class...
	std::string &getDefLanguage();
	std::string &getBackend();

private: // Properties, these should be moved if we make this a base class...
	std::string sDefLang;	/**< The default language for all operations. */
	std::string sBackend;	/**< The backend database plugin name. */
	XmlNode *pBackend;		/**< The xml snippet describing the backend. */
};

#endif
