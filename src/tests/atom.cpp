#include "bu/atom.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct bob
{
	int a, b;
} bob;
int main()
{
	Bu::Atom<int> aInt;
	Bu::Atom<char *> aStr;
	Bu::Atom<bob> aBob;

	aBob = bob();
	aBob->a = 5;

	aStr.set("Hey there, dude");
	aInt.set( 55 );
	int me = aInt;
	aInt = 12;
	printf("%d, %d\n", aInt.get(), me );
	printf("%s\n", aStr.get() );
}

