#include "bu/settings.h"

#include "bu/settingsdrivertaf.h"

Bu::Settings::Settings( const Bu::UtfString &sCompany,
		const Bu::UtfString &sProduct, Bu::Settings::Driver eDriver ) :
	sCompany( sCompany ),
	sProduct( sProduct ),
	pDriver( NULL )
{
	switch( eDriver )
	{
		case DriverNative:
#if defined( WIN32 )
#else
			pDriver = new Bu::SettingsDriverTaf();
#endif
			break;

		case DriverTaf:
			pDriver = new Bu::SettingsDriverTaf();
			break;

		case DriverIni:
			throw Bu::ExceptionBase("Not supported");
			break;
	}

	pDriver->init( sCompany, sProduct );
}

Bu::Settings::~Settings()
{
	delete pDriver;
}

void Bu::Settings::set( const Bu::UtfString &sKey, const Bu::UtfString &sValue )
{
}

