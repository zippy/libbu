#include "sha1.h"
#include "sfile.h"

#define BS 	1024

int main( int argc, char *argv[] )
{
	argc--; argv++;

	if( argc == 0 )
	{
		printf("Provide a filename.\n");
		return 0;
	}

	char buf[BS];

	Sha1 s;
	SFile fin( *argv, "rb" );
	for(;;)
	{
		int nRead = fin.read( buf, BS );
		if( nRead == 0 )
			break;

		s.update( buf, nRead );
		if( nRead < BS )
			break;
	}

	unsigned char *dig = s.getDigest();

	char val[]={"0123456789ABCDEF"};

	for( int j = 0; j < 20; j++ )
	{
		putchar( val[dig[j]>>4] );
		putchar( val[dig[j]&0x0F] );
	}
	putchar('\n');

	delete[] dig;
}

