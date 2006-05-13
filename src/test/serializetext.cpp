#include "serializertext.h"

int main()
{
	SerializerText ar("hello.dat", false);

	ar << 4;
	ar << 3.993;
	ar << true;

	ar.close();

	int one=0;float two=0.0;bool three=false;

	SerializerText ar2("hello.dat", true);
	
	ar2 >> one;
	ar2 >> two;
	ar2 >> three;
	
	printf("out: %d, %d, %s\n", one, two, three);
	
	return 0;
}

