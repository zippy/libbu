#include <stdio.h>
#include "hashfunctionstring.h"

int main( int argc, char *argv[] )
{
	HashFunctionString h;

	printf("\"%s\":  %lu\n", argv[1], h.hash( argv[1] ) );

	return 0;
}

