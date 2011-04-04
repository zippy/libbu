#include <stdint.h>
#include <stdio.h>

void detectEndianness()
{
	uint16_t x=0x0100;
	fprintf( stderr,
		"#define LITTLE_ENDIAN 0\n"
		"#define BIG_ENDIAN 1\n"
		"#define ENDIANNESS %d\n\n",
		((uint8_t *)&x)[0]
		);
	printf("Archetecture is: %s Endian\n", ((uint8_t *)&x)[0]?"Big":"Little" );
}

int main()
{
	fprintf( stderr,
		"#ifndef BU_AUTO_CONFIG_H\n"
		"#define BU_AUTO_CONFIG_H\n\n"
		);

	// huh, turns out #include <endian.h> covers this...
//	detectEndianness();

	fprintf( stderr, "#endif\n");

	return 0;
}

