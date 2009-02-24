#include "bu/sio.h"
#include "bu/file.h"
#include "bu/md5.h"

using Bu::sio;

int main( int argc, char *argv[] )
{
	argv++, argc--;
	for(; *argv; argv++ )
	{
		Bu::File fIn( *argv, Bu::File::Read );
		Bu::Md5 m;

		char buf[100000];
		for(;;)
		{
			int iRead = fIn.read( buf, 100000 );
			m.addData( buf, iRead );
			if( iRead < 100000 )
				break;
		}

		sio << m.getResult() << " *" << *argv << sio.nl;
	}
}
