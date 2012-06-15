#include <bu/myriad.h>
#include <bu/myriadstream.h>
#include <bu/file.h>

int main()
{
	Bu::File f("big.myr", Bu::File::Read|Bu::File::Write|Bu::File::Create );
	Bu::Myriad m( f, 2048 );

	char *buf = new char[1024*1024*10];
	memset( buf, 0, 1024*1024*10 );

	for( int j = 0; j < 250; j++ )
	{
		m.openStream( m.createStream() ).write( buf, 1024*1024*10 );
//		m.sync();
		printf("\r%03d%%", (j+1)*100/250 );
		fflush( stdout );
	}

	printf("\n\n");

	return 0;
}

