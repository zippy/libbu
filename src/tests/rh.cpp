#include "bu/file.h"
#include "bu/hash.h"
#include "bu/archive.h"
#include "bu/fstring.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"

int main( int argc, char *argv[] )
{
	if( argv[1][0] == 'r' )
	{
		typedef Bu::Hash<Bu::FString, int> Hsh;

		Bu::File fIn( argv[2], Bu::File::Read );
		Bu::Archive ar( fIn, Bu::Archive::load );

		Hsh h;
		ar >> h;

		printf("Read %d.\n", h.getSize() );
		for( Hsh::iterator i = h.begin(); i != h.end(); i++ )
		{
			printf(" - \"%s\" = %d\n", i.getKey().getStr(), i.getValue() );
		}

		printf("%d vs. %d\n", h.getSize(), h.getKeys().getSize() );
	}
	else if( argv[1][0] == 'n' )
	{
		typedef Bu::Hash<Bu::FString, int> Hsh;

		Bu::File fIn( argv[2], Bu::File::Read );
		Bu::Nids n( fIn );
		n.initialize();
		Bu::NidsStream sIn = n.openStream( 0 );
		Bu::Archive ar( sIn, Bu::Archive::load );

		Hsh h;
		ar >> h;

		printf("Read %d.\n", h.getSize() );
		for( Hsh::iterator i = h.begin(); i != h.end(); i++ )
		{
			printf(" - \"%s\" = %d\n", i.getKey().getStr(), i.getValue() );
		}

		printf("%d vs. %d\n", h.getSize(), h.getKeys().getSize() );
	}

	return 0;
}

