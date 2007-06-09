#include "bu/bzip2.h"
#include "bu/file.h"

int main( int argc, char *argv[] )
{
	char buf[1024];
	size_t nRead;

	Bu::File f( "test.bz2", "rb" );
	Bu::BZip2 bz2( f );

	Bu::File fin( argv[1], "wb");

	for(;;)
	{
		nRead = bz2.read( buf, 1024 );
		if( nRead > 0 )
			fin.write( buf, nRead );
		if( bz2.isEOS() )
			break;
	}
}

