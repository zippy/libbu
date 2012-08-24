#ifndef BU_SETTINGS_DRIVER_H
#define BU_SETTINGS_DRIVER_H

#include "bu/utfstring.h"

namespace Bu
{
	class Settings;

	/**
	 * Base class for i/o interfaces for Bu::Settings.  By subclassing this you
	 * can provide custom storage for application settings.
	 */
	class SettingsDriver
	{
	friend class Bu::Settings;
	public:
		SettingsDriver();
		virtual ~SettingsDriver();

	protected:
		virtual void init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct )=0;
	};
};

#endif
