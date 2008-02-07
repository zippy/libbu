/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/list.h"
#include <list>

typedef struct Bob
{
	int nID;
} Bob;

int main()
{
	Bu::List<int> l;

	l.append( 0 );

	for( int j = 3; j <= 21; j += 3 )
	{
		l.append( j );
		l.prepend( -j );
	}

	{
		Bu::List<int>::iterator i = l.begin();
		Bu::List<int>::iterator j = i;
		int a, b;
		a = *j;
		printf("end:  %s\n", (j != l.end())?"no":"yes");
		j--;
		printf("end:  %s\n", (j != l.end())?"no":"yes");
		j++;
		printf("end:  %s\n", (j != l.end())?"no":"yes");
		i = j;
		b = *i;
		printf("%d -> %d\n", a, b );
	}

	for( Bu::List<int>::iterator i = l.begin(); i != l.end(); i++ )
	{
		printf("%d ", *i );
	}
	printf("\n");
	for( Bu::List<int>::iterator i = l.begin(); i != l.end(); i++ )
	{
		Bu::List<int>::iterator j = i; j--;
		l.erase( i );
		i = j;
		if( i != l.end() )
			printf("!%d ", *i );
	}

	printf("\n\n");

	Bu::List<Bob> lb;
	for( int j = 0; j < 10; j++ )
	{
		Bob b;
		b.nID = j;
		lb.append( b );
	}

	const Bu::List<Bob> rb = lb;

	for( Bu::List<Bob>::const_iterator i = rb.begin(); i != rb.end(); i++ )
	{
		//i->nID += 2;
		//(*i).nID = 4;
		printf("%d ", i->nID );
	}
	printf("\n\n");
}

