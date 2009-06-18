#include "bu/file.h"
#include "bu/base64.h"

int main( int argc, char *argv[] )
{
	argc--,argv++;

	if( argc < 3 )
		return 0;

	if( argv[0][0] == 'e' )
	{
		argv++;
		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::File fOut( argv[1], Bu::File::WriteNew );
		Bu::Base64 bOut( fOut, Bu::Base64::Write );

		char buf[900];
		for(;;)
		{
			int iRead = fIn.read( buf, 900 );
			bOut.write( buf, iRead );
			if( iRead < 900 )
				break;
		}
	}
	else if( argv[0][0] == 'd' )
	{
		argv++;
		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::File fOut( argv[1], Bu::File::WriteNew );
		Bu::Base64 bIn( fIn, Bu::Base64::Read );

		char buf[900];
		for(;;)
		{
			int iRead = bIn.read( buf, 900 );
			fOut.write( buf, iRead );
			if( iRead < 900 )
				break;
		}
	}

	return 0;
}
