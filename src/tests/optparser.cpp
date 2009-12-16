#include <bu/optparser.h>
#include <bu/sio.h>
using namespace Bu;

class Opts : public Bu::OptParser
{
public:
	Opts() :
		iBob( 542 )
	{
		callback( this, &Opts::cb );
	}

	int cb( int argc, char *argv[] )
	{
		sio << "Hey, cb was called, here's a class var: " << iBob << sio.nl;
		return 5;
	}

	int iBob;
};

int main( int argc, char *argv[] )
{
	Opts o;
}

