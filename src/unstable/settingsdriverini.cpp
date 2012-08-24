#include "bu/settingsdriverini.h"

#include "bu/file.h"
#include "bu/buffer.h"

#include "bu/sio.h"

#include <stdlib.h>

using namespace Bu;

Bu::SettingsDriverIni::SettingsDriverIni()
{
}

Bu::SettingsDriverIni::~SettingsDriverIni()
{
	Bu::File fOut( sPath, Bu::File::WriteNew );
	Bu::Buffer bOut( fOut );
	Bu::Formatter f( bOut );

	for( GroupHash::iterator i = hGroup.begin(); i; i++ )
	{
		f << "[" << i.getKey().get() << "]" << f.nl;
		for( StrHash::iterator k = (*i).begin(); k; k++ )
		{
			f << k.getKey().get() << "=" << k.getValue().get() << f.nl;
		}
		f << f.nl;
	}
}

void Bu::SettingsDriverIni::init( const Bu::UtfString &sCompany,
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
		Bu::Buffer bIn( fIn );
		StrHash hKeys;
		Bu::String sGroup;
		while( !bIn.isEos() )
		{
			Bu::String sIn = bIn.readLine();
			if( sIn[0] == '[' )
			{
				if( !sGroup.isEmpty() )
				{
					hGroup.insert( sGroup, hKeys );
					hKeys.clear();
				}
				sGroup = Bu::String( sIn.begin()+1, sIn.find(']') );
			}
			else
			{
				Bu::String::iterator i = sIn.find('=');
				if( !i )
					continue;

				hKeys.insert( Bu::String( sIn.begin(), i ),
					Bu::String( i+1, sIn.end() ) );
			}
		}
	}
	catch(...)
	{
	}
}

void Bu::SettingsDriverIni::set( const Bu::UtfString &sKey, const Bu::UtfString  &sValue )
{
	Bu::String suKey = sKey.get();
	Bu::String::iterator i = suKey.find('/');
	Bu::UtfString sGrp;
	Bu::UtfString sId;
	if( !i )
	{
		sGrp = "";
		sId = sKey;
	}
	else
	{
		Bu::String::iterator in;
		for(;;)
		{
			in = i;
			i = (in + 1).find('/');
			if( !i )
				break;
		}

		sGrp.set( Bu::String( suKey.begin(), in ) );
		sId.set( Bu::String( in+1, suKey.end() ) );
	}

//	sio << "Group: " << sGrp.get() << sio.nl
//		<< "Key:   " << sId.get() << sio.nl;

	if( !hGroup.has( sGrp ) )
	{
		StrHash hVal;
		hVal.insert( sId, sValue );
		hGroup.insert( sGrp, hVal );
	}
	else
	{
		hGroup.get( sGrp ).insert( sId, sValue );
	}
}

Bu::UtfString Bu::SettingsDriverIni::get( const Bu::UtfString &sKey, const Bu::UtfString &sValue )
{
	Bu::String suKey = sKey.get();
	Bu::String::iterator i = suKey.find('/');
	Bu::UtfString sGrp;
	Bu::UtfString sId;
	if( !i )
	{
		sGrp = "";
		sId = sKey;
	}
	else
	{
		Bu::String::iterator in;
		for(;;)
		{
			in = i;
			i = (in + 1).find('/');
			if( !i )
				break;
		}

		sGrp.set( Bu::String( suKey.begin(), in ) );
		sId.set( Bu::String( in+1, suKey.end() ) );
	}

	sio << "Group: " << sGrp.get() << sio.nl
		<< "Key:   " << sId.get() << sio.nl;

	try
	{
		return hGroup.get( sGrp ).get( sId );
	}
	catch(...)
	{
		return sValue;
	}
}

