#include "param.h"
#include <stdio.h>

Param::Param()
{
	addParam( "name", 's', mkproc( Param::printStuff ), &str );
	//addParam( "name", &str );
	addParam( "job", 'U', mkproc( Param::printStuff ) );

	// --name=Bobo
	// --job hello
}

Param::~Param()
{
}

int Param::printStuff( int argc, char *argv[] )
{
	printf("------------%02d-------------\n", argc );
	for( int j = 0; j < argc; j++ )
	{
		printf("%d: %s\n", j, argv[j] );
	}
	printf("---------------------------\n" );
	printf("SETVAR===\"%s\"\n", str.c_str() );

	return 1;
}

int main( int argc, char *argv[] )
{
	printf("Starting...\n");
	Param p;
	p.process( argc, argv );

	//printf("Calling by hand...\n");
	//p.printStuff();
}

