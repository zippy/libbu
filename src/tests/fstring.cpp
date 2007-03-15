#include "fstring.h"

int main( int argc, char *argv )
{
	FString str("[] this won't be in there", 3);

	str.append("Hello");
	str.append(" th");
	str.append("ere.");

	if( str == "[] Hello there." )
		printf("1) check\n");

	if( str != "[] Hello there. " )
		printf("2) check\n");

	if( str != "[] Hello there." )
		printf("3) failed\n");
	else
		printf("3) check\n");

	str += "  How are you?";

	str.prepend("Bob says:  ");

	printf("%s\n", str.c_str() );
}

