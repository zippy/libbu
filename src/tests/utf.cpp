#include <bu/file.h>
#include <bu/string.h>
#include <bu/utfstring.h>

int main( int argc, char *argv[] )
{
	argc--, argv++;

	for( char **sFile = argv; *sFile; sFile++ )
	{
		Bu::File fIn( *sFile, Bu::File::Read );
		Bu::String sUtf8;
		char buf[4096];
		while( !fIn.isEos() )
		{
			int iAmnt = fIn.read( buf, 4096 );
			sUtf8.append( buf, iAmnt );
		}
		Bu::UtfString::debugUtf8( sUtf8 );
	}
}

