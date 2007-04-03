#include "hash.h"
#include <string.h>

int main()
{
	char *a, *b;
	a = new char[10];
	b = new char[10];
	strcpy( a, "Hey there");
	strcpy( b, "Hey there");
	printf("Same:  %s\n", __cmpHashKeys( a, b )?"yes":"no");

	return 0;
}

