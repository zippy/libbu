#include "serializerbinary.h"
#include "staticstring.h"
#include <stdio.h>
#include <string>

int main()
{
	int32_t one;
	double two;
	bool three;
	StaticString s("Test string!");
	std::string ss("Another test string");
	SerializerBinary ar("hello.dat", false);
	ar << (int)85;
	ar << (double)2.63434;
	ar << false;
	ar << ss;
	ar.close();

	one = 0; two = 0; three = true; s = "die";
	
	SerializerBinary ar2("hello.dat", true);
	ar2 >> one;
	ar2 >> two;
	ar2 >> three;
	ar2 >> s;

	printf("we got %d - %f - %s - \"%s\"\n", one, two, (three ? "true":"false"), s.getString() );
	return 0;
}
