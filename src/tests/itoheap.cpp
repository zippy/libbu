/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bu/itoheap.h"
#include "bu/ito.h"

class Consumer : public Bu::Ito
{
public:
	Consumer()
	{
	}

	virtual ~Consumer()
	{
	}

	void run()
	{
		for( int j = 0; j < 10; j++ )
		{
			printf("Trying to read [%d].\n", j );

			try
			{
				int iNum = hInt.dequeue( 0, 500000 );
				printf("Read %d\n", iNum );
			}
			catch( Bu::HeapException &e )
			{
				printf("Nothing yet...\n");
			}
		}
	}

	Bu::ItoHeap<int> hInt;
};


int main()
{
	Consumer c;

	for( int j = 0; j < 3; j++ )
	{
		int iNum = rand()%10;
		printf("Enqueuing %d.\n", iNum );
		c.hInt.enqueue( iNum );
	}

	printf("Sarting consumer.\n");
	c.start();

	for( int j = 0; j < 5; j++ )
	{
		sleep( 1 );
		int iNum = rand()%10;
		printf("Enqueuing %d.\n", iNum );
		c.hInt.enqueue( iNum );
	}
}

