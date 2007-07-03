#include "formula.h"

int main( int argc, char *argv[] )
{
	if( argc < 2 ) return 0;

	Formula f;
	double dOut = f.run( argv[1] );
	printf("%s = %f\n", argv[1], dOut );
	
	return 0;
}

