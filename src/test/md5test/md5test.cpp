#include <stdio.h>
#include <string.h>
#include "md5.h"

int main()
{
	md5 mproc;
	md5sum sum;
	char hexstr[33];

	memset( hexstr, 0, 33 );

	mproc.sumString( &sum, "qwertyuiopasdfgh" );
	mproc.sumToHex( &sum, hexstr );
	printf("sum: %s\n", hexstr );
	printf("chk: 1ebfc043d8880b758b13ddc8aa1638ef\n");

	return 0;
}
