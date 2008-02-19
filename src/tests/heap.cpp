#include <stdlib.h>
#include <stdio.h>

#include "bu/heap.h"

int main()
{
	Bu::Heap<int> hInt;

	for( int j = 0; j < 15; j++ )
	{
		int r = rand()%10;
		printf("Pushing: %d, top: ", r );
		hInt.push( r );
		printf("%d\n", hInt.peek() );
	}

	for( int j = 0; j < 15; j++ )
	{
		printf("%d ", hInt.peek() );
		hInt.pop();
	}
	printf("\n");
	
//	hInt.print();

	return 0;
}

