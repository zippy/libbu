#include <stdio.h>
#include <stdint.h>

void bitprint( uint16_t u )
{
	for( int i = 15; i >= 0; i-- )
		printf("%c", (u&(1<<i))?'1':'0');
	printf("\n");
}

void bitprint( uint32_t u )
{
	for( int i = 31; i >= 0; i-- )
		printf("%c", (u&(1<<i))?'1':'0');
	printf("\n");
}

void utoutf16( uint32_t in, uint16_t &outHi, uint16_t &outLo )
{
	outHi = (((in-0x10000)>>10)&0x3FF)| 0xD800u;
	outLo = ((in-0x10000)&0x3FF)| 0xDC00u;
	printf("0x%X == 0x%X, 0x%X\n", in, outHi, outLo );
}

int32_t utf16tou( uint16_t hi, uint16_t lo )
{
	return (((uint32_t)hi&0x3FF)<<10 | lo&0x3FF)+0x10000;
}

int main()
{
	bitprint( 0xD800u );
	bitprint( 0xDC00u );
	uint16_t hi, lo;
	utoutf16( 0x1D11E, hi, lo ); // Cat face with wry smile
	utoutf16( 0x10FFFD, hi, lo ); // Cat face with wry smile
	utoutf16( 0x1F63C, hi, lo ); // Cat face with wry smile
	bitprint( hi );
	bitprint( lo );
	printf("0x%X\n", utf16tou( hi, lo ) );
	return 0;
}
