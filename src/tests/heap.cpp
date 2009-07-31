/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "bu/heap.h"

typedef struct num
{
	num( int iNum, int iOrder ) : iNum( iNum ), iOrder( iOrder )
	{
	}

	num( const num &src ) : iNum( src.iNum ), iOrder( src.iOrder )
	{
	}

	int iNum;
	int iOrder;

	bool operator<( const num &oth ) const
	{
		if( iNum == oth.iNum )
			return iOrder < oth.iOrder;
		return iNum < oth.iNum;
	}
	bool operator>( const num &oth ) const
	{
		return iNum > oth.iNum;
	}
} num;

int main()
{
	Bu::Heap<num> hNum;

	for( int j = 0; j < 30; j++ )
	{
		int r = rand()%10;
		printf("Pushing: %d, top: ", r );
		hNum.enqueue( num( r, j ) );
		printf("%d\n", hNum.peek().iNum );
	}

	while( !hNum.isEmpty() )
	{
		printf("(%d:%d) ", hNum.peek().iOrder, hNum.peek().iNum );
		hNum.dequeue();
	}
	printf("\n");

	return 0;
}

