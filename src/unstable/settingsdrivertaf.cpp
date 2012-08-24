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
}

void Bu::SettingsDriverTaf::init( const Bu::UtfString &sCompany,
		const Bu::UtfString &sProduct )
{
	Bu::UtfString us( getenv("HOME") );
	us += "/";
	us += sCompany;
	us += "/";
	us += sProduct;
	Bu::File fIn( us.get(), Bu::File::Read|Bu::File::Create );
	Bu::TafReader tr( fIn );
	pRoot = tr.readGroup();
}

