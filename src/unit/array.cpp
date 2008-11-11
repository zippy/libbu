/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/hash.h"
#include "bu/array.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("Array");
		addTest( Unit::general );
		addTest( Unit::iterate1 );
		addTest( Unit::iterate2 );
		addTest( Unit::copy );
	}

	virtual ~Unit()
	{
	}

	void general()
	{
		Bu::Array<int> ai;

		ai.append( 5 );
		ai.append( 10 );
		unitTest( ai.getSize() == 2 );
		unitTest( ai.getCapacity() == 10 );
		unitTest( ai[0] == 5 );
		unitTest( ai[1] == 10 );
	}

	void iterate1()
	{
		Bu::Array<int> ai;
		for( int j = 0; j < 10; j++ )
			ai.append( j );

		int j = 0;
		for( Bu::Array<int>::iterator i = ai.begin(); i != ai.end(); i++ )
			unitTest( (*i) == j++ );
		
		const Bu::Array<int> &ci = ai;
		j = 0;
		for( Bu::Array<int>::const_iterator i = ci.begin(); i != ci.end(); i++ )
			unitTest( (*i) == j++ );
	}

	void iterate2()
	{
		Bu::Array<int> ai;
		for( Bu::Array<int>::iterator i = ai.begin(); i != ai.end(); i++ )
			unitFailed("Empty lists shouldn't be iterated through.");
	}

	void copy()
	{
		typedef Bu::Hash<Bu::FString, Bu::FString> StrHash;
		typedef Bu::Array<StrHash> StrHashArray;

		StrHash h1;
		h1["Hi"] = "Yo";
		h1["Bye"] = "Later";

		StrHash h2;
		h2["Test"] = "Bloop";
		h2["Foo"] = "ooF";

		StrHashArray a1;
		a1.append( h1 );
		a1.append( h2 );

		StrHashArray a2(a1);

		unitTest( a2[0].get("Hi") == "Yo" );
		unitTest( a2[0].get("Bye") == "Later" );
		unitTest( a2[1].get("Test") == "Bloop" );
		unitTest( a2[1].get("Foo") == "ooF" );

		StrHashArray a3;
		a3 = a1;

		unitTest( a3[0].get("Hi") == "Yo" );
		unitTest( a3[0].get("Bye") == "Later" );
		unitTest( a3[1].get("Test") == "Bloop" );
		unitTest( a3[1].get("Foo") == "ooF" );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

