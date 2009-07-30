#include "bu/membuf.h"
#include "bu/buffer.h"
#include "bu/file.h"

using namespace Bu;

int main( int argc, char *argv[] )
{
	argc--,argv++;
	if( argc == 0 )
	{
		MemBuf mOut;
		Buffer bOut( mOut, 10 );

		for( int j = 0; j < 4; j++ )
			bOut.write("hi ", 3 );

		printf("Preflush:  \"%s\"\n", mOut.getString().getStr() );
		bOut.flush();

		printf("Final:     \"%s\"\n", mOut.getString().getStr() );
	}
	else
	{
		File fIn( *argv, File::Read );
		Buffer bIn( fIn, 10 );

		char buf[5];
		for( int j = 0; j < 5; j++ )
		{
			buf[bIn.read( buf, 4 )] = '\0';
			printf("Four chars:  \"%s\"\n", buf );
		}
	}

	return 0;
}

