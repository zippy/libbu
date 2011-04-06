#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

FILE *fOut = NULL;

bool testCpp( const char *prog )
{
	FILE *pop = popen("g++ -x c++ - -o /dev/null", "w");
	fwrite( prog, 1, strlen( prog ), pop );
	return pclose(pop) == 0;
}

void detectEndianness()
{
	printf("Detecting endian support...");
	fflush( stdout );
	if( testCpp("#include <endian.h>\n\nint main() { return BYTE_ORDER; }\n") )
	{
		fprintf( fOut, "#include <endian.h>\n\n");
		printf("header file endian.h found, using that.\n");
	}
	else
	{
		uint16_t x=0x0100;
		fprintf( fOut,
			"#define LITTLE_ENDIAN 0\n"
			"#define BIG_ENDIAN 1\n"
			"#define BYTE_ORDER %d\n\n",
			((uint8_t *)&x)[0]
			);
		printf("no header file found, faking it...\n"
			"\tArchetecture is: %s Endian\n",
			((uint8_t *)&x)[0]?"Big":"Little"
			);
	}
}

int main( int argc, char *argv[] )
{
	if( argc == 1 )
	{
		fprintf( stderr,
			"Invalid usage: specify a file to generate:\n"
			"    src/autoconfig.h\n"
			"    src/version.h\n"
			"\n"
			);
		return 127;
	}
	if( strcmp( argv[1], "src/autoconfig.h" ) == 0 )
	{
		fOut = fopen( argv[1], "w" );
		fprintf( fOut,
			"#ifndef BU_AUTO_CONFIG_H\n"
			"#define BU_AUTO_CONFIG_H\n\n"
			);

		detectEndianness();

		fprintf( fOut, "#endif\n");
	}
	else if( strcmp( argv[1], "src/version.h" ) == 0 )
	{
		fOut = fopen( argv[1], "w" );
		fprintf( fOut,
			"#ifndef BU_VERSION_H\n"
			"#define BU_VERSION_H\n\n"
			"#define LIBBU_VERSION		0\n"
			"#define LIBBU_REVISION		1\n"
			"#define LIBBU_VERSION_STR	\"0.1\"\n"
			"#define LIBBU_API			0\n"
			"#define LIBBU_VC_ID		\"");
		FILE *psub = popen("svnversion -n", "r");
		char buf[1024];
		fwrite( buf, fread( buf, 1, 1024, psub ), 1, fOut );
		pclose( psub );
		fprintf( fOut, "\"\n\n#endif\n");
	}

	return 0;
}

