#include "bu/process.h"

#include <stdio.h>

int main()
{
	//Bu::Process p( argv[1], argv+1 );
	    Bu::Process p("mplayer", "mplayer", "dvd://", "-framedrop",
        "-ao", "null", "-nosound", "-vf", "framestep=I,cropdetect" "-sstep",
        "197", NULL );

	char buf[1000];
	for(;;)
	{
		int iSize = p.read( buf, 1000 );
		printf("::read=%d::\n", iSize );
		if( iSize == 0 )
			break;
		fwrite( buf, iSize, 1, stdout );
		if( iSize < 1000 )
			break;
	}

	return 0;
}

