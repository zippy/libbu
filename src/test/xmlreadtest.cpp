#include "../xmlfilereader.h"
#include "../xmlstringreader.h"
#include "../xmlfilewriter.h"

int main( int argc, char *argv[] )
{
	if( argc < 4 )
	{
		printf("Usage: %s f <file in> <file out>\n", argv[0] );
		printf("       %s s <xml string> <file out>\n\n", argv[0] );
		return 0;
	}

	if( argv[1][0] == 'f' )
	{
		XmlFileReader r( argv[2], true );
		XmlFileWriter w( argv[3], "\t", r.detatchRoot() );
		w.write();
	}
	else if( argv[1][0] == 's' )
	{
		XmlStringReader r( argv[2], true );
		XmlWriter w( argv[3], "\t", r.detatchRoot() );
		w.write();
	}

	return 0;
}

