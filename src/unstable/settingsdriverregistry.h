#ifndef BU_SETTINGS_DRIVER_REGISTRY_H
#define BU_SETTINGS_DRIVER_REGISTRY_H

#ifdef WIN32

#include "bu/settingsdriver.h"

namespace Bu
{
    class SettingsDriverRegistry : public SettingsDriver
    {
    public:
        SettingsDriverRegistry();
        virtual ~SettingsDriverRegistry();

    protected:
        virtual void init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct );
        virtual void set( const Bu::UtfString &sKey, const Bu::UtfString  &sValue );
        virtual Bu::UtfString get( const Bu::UtfString &sKey, const Bu::UtfString &sValue );

    private:
        void *rphKey;
    };
};

#endif

#endif
