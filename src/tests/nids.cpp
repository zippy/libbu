#include "bu/file.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"

int main( int argc, char *argv[] )
{
	if( argc < 2 )
	{
		printf("usage: %s <output>\n\n", argv[0] );
		return 1;
	}

	Bu::File fOut( argv[1], "wb+");
	Bu::Nids n( fOut );

//	n.initialize( 120, 5 );

	Bu::NidsStream s = n.openStream( n.createStream() );
/*
	Bu::FString sBuf( 350 );
	memset( sBuf.getStr(), 'a', 350 );
	s.write( sBuf );
	*/

	return 0;
}

