
#define BU_TRACE
#include "bu/trace.h"

void doThing3( int x, const char *bob, void *p )
{
	TRACE( x, bob, p );
}

void doThing2( int x, const char *bob )
{
	TRACE( x, bob );
}

void doThing( int8_t x )
{
	TRACE( x );
}

int main( int argc, char *argv[] )
{
	TRACE( argc, argv );
	doThing( 54 );
	doThing2( 128, "Hello" );
	doThing3( 266, "Goodbye", argv );

	return 0;
}

