#include <bu/optparser.h>
#include <bu/sio.h>
using namespace Bu;

class Opts : public Bu::OptParser
{
public:
	Opts() :
		iBob( 542 )
	{
		addHelpBanner("optparser - Test some option things...");

		addHelpBanner("\nThis section represents options that actually have "
			"callbacks, or in the case of the new system, signals/slots.  They "
			"all take parameters, but if they return 0 then it will be as "
			"though they hadn't and the next thing will be processed normally.",
			true
			);
		addOption( slot( this, &Opts::yesparam ), 'x', "things",
			"This is the first test parameter.  It calls a function, and "
			"takes a parameter."
			);
		addOption( slot( this, &Opts::noparam ), 'y', "stuff",
			"This is the second test parameter.  It does not take "
			"parameters.  However, I do want to make this part much longer to "
			"see how it looks when you add way too much text to one of these "
			"things.  It can't really be that bad, right?"
			);

		addHelpBanner("\nThis section represents options with no callback or "
			"signal, but do have a variable to update.  They use the Formatter "
			"system and therefore it's very, very flexible.  Any data type "
			"you can read with a formatter you can set via parameter.",
			true
			);
		addOption( sVar, 's', "str", "Set a variable, see what it does.");
		addOption( iBob, "bob", "Change iBob to wahtever you want.");
		addOption( dBob, 'd', "Change dBob to wahtever you want.");

		setOverride("str", "Bob!");
	
		addHelpOption();

		setNonOption( slot( this, &Opts::nonOption ) );
	}

	int yesparam( StrArray aParams )
	{
		sio << " - yesparam" << aParams << sio.nl;
		return 1;
	}

	int noparam( StrArray aParams )
	{
		sio << " - noparam" << aParams << sio.nl;
		return 0;
	}

	int nonOption( StrArray aParams )
	{
		sio << " - nonOption" << aParams << sio.nl;
		return aParams.getSize()-1;
	}

	int iBob;
	float dBob;
	Bu::FString sVar;
};

int main( int argc, char *argv[] )
{
	Opts o;

	o.parse( argc, argv );

	sio << "sVar = \"" << o.sVar << "\"" << sio.nl;
	sio << "iBob = " << o.iBob << sio.nl;
	sio << "dBob = " << o.dBob << sio.nl;
}
