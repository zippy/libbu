#ifdef WIN32

#include "bu/settingsdriverregistry.h"
#include <windows.h>
#include "bu/string.h"

#define phKey ((HKEY *)rphKey)

Bu::SettingsDriverRegistry::SettingsDriverRegistry()
{
}

Bu::SettingsDriverRegistry::~SettingsDriverRegistry()
{
	RegCloseKey( *phKey );
}

void Bu::SettingsDriverRegistry::init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct )
{
	Bu::UtfString us("Software\\");
	us += sCompany;
	us += "\\";
	us += sProduct;
	rphKey = new HKEY;
	RegCreateKeyExA( HKEY_CURRENT_USER, us.get().getStr(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, phKey, NULL );
}

void Bu::SettingsDriverRegistry::set( const Bu::UtfString &sKey, const Bu::UtfString  &sValue )
{
	Bu::StringList lPath = sKey.get().split('/');
	Bu::StringList::iterator i = lPath.begin();
	Bu::StringList::iterator in;

	Bu::String sPKey;
	for(; i;)
	{
		in = i;
		in++;
		if( in )
		{
			if( !sPKey.isEmpty() )
				sPKey += "\\";
			sPKey += *i;
		}
		i = in;
	}

	HKEY key;
	RegCreateKeyExA( *phKey, sPKey.getStr(), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL );
	Bu::String sTmp = sValue.get();
	RegSetValueExA( key, lPath.last().getStr(), 0, REG_SZ, (BYTE *)sTmp.getStr(), sTmp.getSize()+1 ); 
	RegCloseKey( key );
}

Bu::UtfString Bu::SettingsDriverRegistry::get( const Bu::UtfString &sKey, const Bu::UtfString &sValue )
{
	Bu::StringList lPath = sKey.get().split('/');
	Bu::StringList::iterator i = lPath.begin();
	Bu::StringList::iterator in;

	Bu::String sPKey;
	for(; i;)
	{
		in = i;
		in++;
		if( in )
		{
			if( !sPKey.isEmpty() )
				sPKey += "\\";
			sPKey += *i;
		}
		i = in;
	}

	HKEY key;
	if( RegOpenKeyExA( *phKey, sPKey.getStr(), 0, KEY_ALL_ACCESS, &key ) 
		!= ERROR_SUCCESS )
		return sValue;
	char buf[4096];
	DWORD iRet = 4096;
	if( RegQueryValueEx( key, lPath.last().getStr(), NULL, NULL, (BYTE *)buf, &iRet ) != ERROR_SUCCESS )
	{
		RegCloseKey( key );
		return sValue;
	}
	RegCloseKey( key );
	
	return Bu::UtfString( Bu::String( buf, iRet ) );
}

#endif
