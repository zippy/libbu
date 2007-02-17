#include "formula.h"

int main( int argc, char *argv[] )
{
	if( argc < 2 ) return 0;

	Formula f;
	double dOut = f.run( argv[1] );
	printf("Final:  %f\n", dOut );
	
	return 0;
}

