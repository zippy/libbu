#include "hash.h"
#include "fstring.h"

FString genThing()
{
	FString bob;
	bob.append("ab ");
	bob += "cd ";
	bob += "efg";

    printf("---bob------\n%08X: %s\n", (unsigned int)bob.c_str(), bob.c_str() );
	return bob;
}

void thing( FString str )
{
	printf("Hey:  %s\n", str.c_str() );
}

#define pem printf("---------\n%08X: %s\n%08X: %s\n", (unsigned int)str.c_str(), str.c_str(), (unsigned int)str2.c_str(), str2.c_str() );
int main( int argc, char *argv )
{
	FString str("th");

	str.prepend("Hello ");
	str.append("ere.");

	FString str2( str );
	pem;
	str += "  What's up?";
	pem;
	str2 += "  How are you?";
	pem;
	str = str2;
	pem;

	str2 = genThing();
	pem;

	str = str2;
	pem;

	thing( str2 );
	thing("test.");
	
	printf("%d == %d\n", __calcHashCode( str ), __calcHashCode( str.c_str() ) );
}

