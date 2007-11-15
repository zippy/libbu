/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/hash.h"
#include "bu/sptr.h"

typedef struct Bob
{
	int nID;
} Bob;

int main()
{
	Bu::Hash<int, Bu::SPtr<const Bob> > lb;
	for( int j = 0; j < 10; j++ )
	{
		Bob *b = new Bob;
		b->nID = j;
		lb.insert( j, b );
	}

	for( int j = 0; j < 10; j++ )
	{
		printf("%d\n", lb[j].value()->nID );
	}
}

