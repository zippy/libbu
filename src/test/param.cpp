#include "param.h"
#include <stdio.h>

Param::Param()
{
	addHelpBanner("param - A test of the libbu++ parameter systems\n"
			"Enjoy with care and caution\n\nTest stuff:\n");
	addParam( "name", 's', mkproc( Param::printStuff ), &str, "Test a param param" );
	//addParam( "name", &str );
	addParam( "job", 'U', mkproc( Param::printStuff ), "Test a paramless param" );

	addHelpBanner("\nInformational:\n");
	addParam( "help", mkproc( ParamProc::help ), "Help!" );

	addHelpBanner("\nThanks for trying my test!\n\n");
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
	if( argc == 1 )
	{
		printf("You have to enter some parameter, try '--help'\n\n");
		return 0;
	}

	Param p;
	p.process( argc, argv );
}

