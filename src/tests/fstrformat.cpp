#include "bu/fstring.h"
#include <stdio.h>

int main()
{
	Bu::FString s;

	s.format("%d, %f, \'%s\'", 144, 12.5, "bob" );

	printf("test:  %s\n", s.getStr() );
}

