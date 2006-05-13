#include "serializerbinary.h"
#include <stdio.h>

int main()
{
	int32_t one;
	double two;
	bool three;
	SerializerBinary ar("hello.dat", false);
	ar << (int)85;
	ar << (double)2.63434;
	ar << false;
	ar.close();

	one = 0; two = 0; three = true;
	
	SerializerBinary ar2("hello.dat", true);
	ar2 >> one;
	ar2 >> two;
	ar2 >> three;

	printf("we got %d - %f - %s\n", one, two, (three ? "true":"false"));
	return 0;
}
