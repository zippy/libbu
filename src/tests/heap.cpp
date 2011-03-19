/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "bu/formatter.h"
#include "bu/heap.h"
#include "bu/string.h"
#include "bu/file.h"

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

void printHeap( Bu::Heap<Bu::String> &h, int j )
{
//	return;
	Bu::String sFName = Bu::String("graph-step-%1.dot").arg( j, Bu::Fmt().width(2).fill('0') );
	Bu::File fOut( sFName, Bu::File::WriteNew );
	Bu::Formatter f( fOut );
	f << "Graph step: " << j << ", total size: " << h.getSize() << f.nl;
	for( Bu::Heap<Bu::String>::iterator i = h.begin(); i; i++ )
	{
		f << *i << f.nl;
	}
	f << f.nl;
}

int main()
{
	/*
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
*/
	Bu::Heap<Bu::String> hStr;
	int j = 0;

	hStr.enqueue("George");
	printHeap( hStr, j++ );
	hStr.enqueue("George");
	printHeap( hStr, j++ );
	hStr.enqueue("Sam");
	printHeap( hStr, j++ );
	hStr.enqueue("Abby");
	printHeap( hStr, j++ );
	hStr.enqueue("Zorro");
	printHeap( hStr, j++ );
	hStr.enqueue("Brianna");
	printHeap( hStr, j++ );
	hStr.enqueue("Kate");
	printHeap( hStr, j++ );
	hStr.enqueue("Soggy");
	printHeap( hStr, j++ );

	while( !hStr.isEmpty() )
	{
		printf("\"%s\" ", hStr.dequeue().getStr() );
		printHeap( hStr, j++ );
	}
	printf("\n");

	Bu::List<Bu::String> lStr;

	lStr.insertSorted("George");
	lStr.insertSorted("George");
	lStr.insertSorted("Sam");
	lStr.insertSorted("Abby");
	lStr.insertSorted("Zorro");
	lStr.insertSorted("Brianna");
	lStr.insertSorted("Kate");
	lStr.insertSorted("Soggy");
	for( Bu::List<Bu::String>::iterator i = lStr.begin(); i; i++ )
	{
		printf("\"%s\" ", (*i).getStr() );
	}
	printf("\n");

	return 0;
}

