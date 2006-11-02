#include "sbuffer.h"

int main()
{
	SBuffer buf;

	buf.write("abcdefg", 7 );

	printf("tell: %d\n", buf.tell() );

	char abuf[6];
	int nRead;
	nRead = buf.read( abuf, 5 );
	abuf[nRead] = '\0';
	printf("Read %d bytes \"%s\"\n", nRead, abuf );

	buf.setPos( 0 );
	nRead = buf.read( abuf, 5 );
	abuf[nRead] = '\0';
	printf("Read %d bytes \"%s\"\n", nRead, abuf );

	nRead = buf.read( abuf, 5 );
	abuf[nRead] = '\0';
	printf("Read %d bytes \"%s\"\n", nRead, abuf );

}

