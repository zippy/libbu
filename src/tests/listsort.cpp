/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/list.h>
#include <bu/sio.h>
#include <bu/string.h>

using namespace Bu;

int main()
{
	/*
	List<int> il;
	il.append( 5 );
	il.append( 12 );
	il.append( 0 );
	il.append( 7 );
	il.append( 3 );
	il.append( 5 );
	Bu::__basicLTCmp<int> cmp;
	il.sortI( cmp );
	*/

	String a("Soggy"), b("Sam");

	if( a < b )
	{
		sio << "Bad" << sio.nl;
	}
	else
	{
		sio << "Good" << sio.nl;
	}

	typedef List<String> StrList;

	StrList lNames;

	lNames.append("George");
	lNames.append("Sam");
	lNames.append("Abby");
	lNames.append("Zorro");
	lNames.append("Brianna");
	lNames.append("Kate");
	lNames.append("Soggy");

	sio << "Names: " << lNames << sio.nl;
	lNames.sort();
	
	sio << "Names: " << lNames << sio.nl;

	StrList lNames2;
	
	lNames2.insertSorted("George");
	lNames2.insertSorted("Sam");
	lNames2.insertSorted("Abby");
	lNames2.insertSorted("Zorro");
	lNames2.insertSorted("Brianna");
	lNames2.insertSorted("Kate");
	lNames2.insertSorted("Soggy");

	sio << "Names: " << lNames2 << sio.nl;

	if( lNames == lNames2 )
	{
		sio << "They're the same." << sio.nl;
	}
	else
	{
		sio << "They're different." << sio.nl;
	}

	return 0;
}

