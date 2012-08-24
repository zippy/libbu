#ifndef BU_SETTINGS_DRIVER_TAF_H
#define BU_SETTINGS_DRIVER_TAF_H

#include "bu/settingsdriver.h"
#include "bu/string.h"

namespace Bu
{
	class TafGroup;

	/**
	 * The taf driver is flawed until I fix taf editing, I've been meaning to...
	 */
	class SettingsDriverTaf : public SettingsDriver
	{
	public:
		SettingsDriverTaf();
		virtual ~SettingsDriverTaf();

	protected:
		virtual void init( const Bu::UtfString &sCompany, const Bu::UtfString &sProduct );
		virtual void set( const Bu::UtfString &sKey, const Bu::UtfString  &sValue );		
		virtual Bu::UtfString get( const Bu::UtfString &sKey, const Bu::UtfString &sValue );

	private:
		Bu::String sPath;
		class Bu::TafGroup *pRoot;
	};
};

#endif
