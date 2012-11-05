#ifndef BU_SETTINGS_DRIVER_INI_H
#define BU_SETTINGS_DRIVER_INI_H

#include "bu/settingsdriver.h"
#include "bu/string.h"
#include "bu/hash.h"

namespace Bu
{
    class SettingsDriverIni : public SettingsDriver
    {
    public:
        SettingsDriverIni();
        virtual ~SettingsDriverIni();

    protected:
        virtual void init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct );
        virtual void set( const Bu::UtfString &sKey, const Bu::UtfString  &sValue );        
        virtual Bu::UtfString get( const Bu::UtfString &sKey, const Bu::UtfString &sValue );

    private:
        Bu::String sPath;
        typedef Bu::Hash<Bu::UtfString, Bu::UtfString> StrHash;
        typedef Bu::Hash<Bu::UtfString, StrHash> GroupHash;
        GroupHash hGroup;
    };
};

#endif
