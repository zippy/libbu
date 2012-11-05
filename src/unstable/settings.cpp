#include "bu/settings.h"

#include "bu/settingsdrivertaf.h"
#include "bu/settingsdriverregistry.h"
#include "bu/settingsdriverini.h"

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
            pDriver = new Bu::SettingsDriverRegistry();
#else
            pDriver = new Bu::SettingsDriverIni();
#endif
            break;

        case DriverTaf:
            pDriver = new Bu::SettingsDriverTaf();
            break;

        case DriverIni:
            pDriver = new Bu::SettingsDriverIni();
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
    pDriver->set( sKey, sValue );
}

Bu::UtfString Bu::Settings::get( const Bu::UtfString &sKey,
        const Bu::UtfString &sValue )
{
    return pDriver->get( sKey, sValue );
}

