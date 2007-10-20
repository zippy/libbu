#include "bu/formula.h"

int main( int argc, char *argv[] )
{
	Bu::Formula<uint32_t> uForm;
	Bu::Formula<double> dForm;

	printf("u: %s = %u\n", argv[1], uForm.run( argv[1] ) );
	printf("d: %s = %f\n", argv[1], dForm.run( argv[1] ) );
}

