/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/ringbuffer.h"
#include <stdlib.h>

int main()
{
	Bu::RingBuffer<uint32_t> ibuf( 10 );

	for( int k = 0; k < 2; k++ )
	{
		int j = 1;
		for(; j < 7; j++ )
		{
			ibuf.enqueue( j );
		}

		for(; j < 20; j++ )
		{
			ibuf.enqueue( j );
			printf("- %d\n", ibuf.dequeue() );
		}

		for(;;)
		{
			if( ibuf.isEmpty() ) break;
			printf(". %d\n", ibuf.dequeue() );
		}
	}
}

