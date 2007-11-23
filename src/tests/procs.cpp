#include "bu/process.h"

#include <stdio.h>

int main( int agrc, char *argv[] )
{
	Bu::Process p( argv[1], argv+1 );

	char buf[1000];
	for(;;)
	{
		int iSize = p.read( buf, 1000 );
		if( iSize == 0 )
			break;
		fwrite( buf, iSize, 1, stdout );
		if( iSize < 1000 )
			break;
	}

	return 0;
}

