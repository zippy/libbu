#include "bu/settingsdrivertaf.h"

#include "bu/file.h"
#include "bu/taf.h"

#include <stdlib.h>

Bu::SettingsDriverTaf::SettingsDriverTaf() :
    pRoot( NULL )
{
}

Bu::SettingsDriverTaf::~SettingsDriverTaf()
{
    if( !pRoot )
        return;

    Bu::File fOut( sPath, Bu::File::WriteNew );
    Bu::TafWriter tw( fOut );
    tw.writeGroup( pRoot );
    delete pRoot;
}

void Bu::SettingsDriverTaf::init( const Bu::UtfString &sCompany,
        const Bu::UtfString &sProduct )
{
    Bu::UtfString us( getenv("HOME") );
    us += "/.config/";
    us += sCompany;
    us += "/";
    us += sProduct;
    sPath = us.get();
    try
    {
        Bu::File fIn( sPath, Bu::File::Read|Bu::File::Create );
        Bu::TafReader tr( fIn );
        pRoot = tr.readGroup();
    }
    catch(...)
    {
    }
    if( !pRoot )
    {
        pRoot = new Bu::TafGroup( sProduct.get() );
    }
}

void Bu::SettingsDriverTaf::set( const Bu::UtfString &sKey,
        const Bu::UtfString &sValue )
{
    Bu::StringList lPath = sKey.get().split('/');
    Bu::StringList::iterator i = lPath.begin();
    Bu::StringList::iterator in;
    Bu::TafGroup *pGrp = pRoot;
    for(; i;)
    {
        in = i;
        in++;
        if( in )
        {
            if( pGrp->hasChild( *i ) )
                pGrp = (Bu::TafGroup *)pGrp->getChild( *i );
            else
                pGrp = pGrp->addGroup( *i );
        }
        else
        {
            pGrp->addProperty( *i, sValue.get() );
        }
        i = in;
    }
}

Bu::UtfString Bu::SettingsDriverTaf::get( const Bu::UtfString &sKey,
        const Bu::UtfString &sValue )
{
    Bu::StringList lPath = sKey.get().split('/');
    Bu::StringList::iterator i = lPath.begin();
    Bu::StringList::iterator in;
    Bu::TafGroup *pGrp = pRoot;
    for(; i;)
    {
        in = i;
        in++;
        if( in )
        {
            if( pGrp->hasChild( *i ) )
                pGrp = (Bu::TafGroup *)pGrp->getChild( *i );
            else
                return sValue;
        }
        else
        {
            if( pGrp->hasProperty( *i ) )
                return pGrp->getProperty( *i );
            else
                return sValue;
        }
        i = in;
    }
}

