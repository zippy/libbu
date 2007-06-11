#include "bu/bzip2.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
	char buf[1024];
	size_t nRead;

	Bu::File f( "test.bz2", "wb" );
	Bu::BZip2 bz2( f );

	Bu::File fin( argv[1], "rb");

	for(;;)
	{
		nRead = fin.read( buf, 1024 );
		if( nRead > 0 )
			bz2.write( buf, nRead );
		if( fin.isEOS() )
			break;
	}
}

