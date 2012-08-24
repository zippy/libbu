#include <bu/file.h>
#include <bu/string.h>
#include <bu/utfstring.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	sio << "Code: " << Bu::__calcHashCode( Bu::UtfString("Hello there") )
		<< sio.nl;

	Bu::File fIn("utf8.in", Bu::File::Read );
	Bu::String sUtf8;
	char buf[4096];
	while( !fIn.isEos() )
	{
		int iAmnt = fIn.read( buf, 4096 );
		sUtf8.append( buf, iAmnt );
	}
	Bu::UtfString us( sUtf8, Bu::UtfString::Utf8 );
	us.debug();
	{
		Bu::File fOut("utf8.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf8 );
	}
	{
		Bu::File fOut("utf16.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf16 );
	}
	{
		Bu::File fOut("utf16le.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf16le );
	}
	{
		Bu::File fOut("utf16be.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf16be );
	}
	{
		Bu::File fOut("utf32.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf32 );
	}
	{
		Bu::File fOut("utf32le.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf32le );
	}
	{
		Bu::File fOut("utf32be.out", Bu::File::WriteNew );
		us.write( fOut, Bu::UtfString::Utf32be );
	}

	/*
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
		Bu::UtfString us( sUtf8, Bu::UtfString::Utf16 );

		us.debug();
	}
	*/
}

