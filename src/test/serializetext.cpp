#include "serializertext.h"
#include "staticstring.h"
#include <iostream>

int main()
{
	StaticString s("You're a dog!!");
	SerializerText ar("hello.dat", false);

	ar << 4 << 3.993 << true << s;

	ar.close();

	int one=0;float two=0.0;bool three=false; s = "";

	SerializerText ar2("hello.dat", true);
	
	ar2 >> one;
	ar2 >> two;
	ar2 >> three;
	ar2 >> s;
	
	//printf("out: %d, %f, %s, \"%s\"\n", one, two, (three ? "true" : "false"), s.getString());
	std::cout << one << ", " << two << ", " << three << ", " << s.getString() << "\n";
	
	return 0;
}

