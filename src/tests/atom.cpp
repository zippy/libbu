#include "bu/atom.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	Bu::Atom<int> aInt;
	Bu::Atom<char *> aStr;

	aStr.set("Hey there, dude");
	aInt.set( 55 );
	int me = aInt;
	aInt = 12;
	printf("%d, %d\n", aInt.get(), me );
	printf("%s\n", aStr.get() );
}

