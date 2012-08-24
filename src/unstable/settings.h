#ifndef BU_SETTINGS_H
#define BU_SETTINGS_H

#include "bu/utfstring.h"

namespace Bu
{
	/**
	 * Simple access to configuration data.  Provides a consistant, cross
	 * platform interface to configuration data using native storage.
	 */
	class Settings
	{
	public:
		enum Driver
		{
			DriverNative,
			DriverTaf,
			DriverIni
		};
		Settings( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct,
			Driver driver=DriverNative );
		virtual ~Settings();

		void set( const Bu::UtfString &sKey, const Bu::UtfString &sValue );

	private:
		Bu::UtfString sCompany;
		Bu::UtfString sProduct;
		class SettingsDriver *pDriver;
	};
};

#endif
