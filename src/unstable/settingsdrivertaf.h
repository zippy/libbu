#ifndef BU_SETTINGS_DRIVER_TAF_H
#define BU_SETTINGS_DRIVER_TAF_H

#include "bu/settingsdriver.h"

namespace Bu
{
	class TafGroup;

	class SettingsDriverTaf : public SettingsDriver
	{
	public:
		SettingsDriverTaf();
		virtual ~SettingsDriverTaf();

	protected:
		virtual void init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct );

	private:
		class Bu::TafGroup *pRoot;
	};
};

#endif
