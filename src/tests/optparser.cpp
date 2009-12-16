#include <bu/optparser.h>
#include <bu/sio.h>
using namespace Bu;

class Opts : public Bu::OptParser
{
public:
	Opts() :
		iBob( 542 )
	{
		Option o;
		o.sUsed = slot( this, &Opts::cb );
		o.cOpt = 'x';
		o.sOpt = "things";
		o.bShortHasParams = true;
		o.sHelp = "This is the first test parameter.  It calls a function, and takes parameters.";
		addOption( o );
		
		Option o2;
		o2.sUsed = slot( this, &Opts::cb );
		o2.cOpt = 'y';
		o2.sOpt = "stuff";
		o2.bShortHasParams = false;
		o2.sHelp = "This is the second test parameter.  It does not take parameters.  However, I do want to make this part much longer to see how it looks when you add way too much text to one of these things.  It can't really be that bad, right?";
		addOption( o2 );
	
		addHelpOption('h', "help", "This help.");
	}

	int cb( StrArray aParams )
	{
		sio << "Hey, cb was called, here's a class var: " << iBob << sio.nl;
		sio << "argv[] = " << aParams << sio.nl;
		return 1;
	}

	int iBob;
};

int main( int argc, char *argv[] )
{
	Opts o;

	o.parse( argc, argv );
}

