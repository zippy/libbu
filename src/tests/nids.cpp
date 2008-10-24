#include "bu/file.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"

int main( int argc, char *argv[] )
{
	if( argc < 3 )
	{
		printf("usage: %s [r|w] <output>\n\n", argv[0] );
		return 1;
	}

	if( argv[1][0] == 'w' )
	{
		Bu::File fOut( argv[2],
			Bu::File::ReadWrite|Bu::File::Create|Bu::File::Truncate );
		Bu::Nids n( fOut );

		n.initialize( 120, 1 );
		Bu::NidsStream s = n.openStream( n.createStream() );

		Bu::FString sBuf( 350 );
		memset( sBuf.getStr(), 'a', 350 );
		s.write( sBuf );
	}
	else if( argv[1][0] == 'r' )
	{
		Bu::File fOut( argv[2], Bu::File::Read );
		Bu::Nids n( fOut );

		Bu::NidsStream s = n.openStream( 0 );
		char buf[75];
		for( int j = 0; j < 3; j++ )
		{
			int iRead = s.read( buf, 75 );
			fwrite( buf, 1, iRead, stdout );
			printf("\n(read %d chars)\n", iRead );
		}
		printf("Position:  %ld\n", s.tell() );
	}
	else
	{
		printf("r or w, those are your choices.\n");
	}

	return 0;
}
