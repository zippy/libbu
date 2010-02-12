#include "bu/optparser.h"
#include "bu/file.h"
#include "bu/newline.h"
#include "bu/csvreader.h"
#include "bu/buffer.h"
#include "bu/sio.h"

using namespace Bu;

class Options : public OptParser
{
public:
	Options( int argc, char *argv[] )
	{
		addOption( slot( this, &Options::onRead ), 'r', "read",
				"Read and display a csv file." );

		addHelpOption();

		parse( argc, argv );
	}

	int onRead( StrArray aArgs )
	{
		File fIn( aArgs[1], File::Read );
		NewLine nlIn( fIn );
		Buffer bIn( nlIn );
		CsvReader rCsv( bIn );
		while( !fIn.isEos() )
		{
			sio << rCsv.readLine() << sio.nl;
		}
		sio << sio.nl;
		return 1;
	}
};

int main( int argc, char *argv[] )
{
	Options opts( argc, argv );
	return 0;
}

