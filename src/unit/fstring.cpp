/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "fstring.h"
#include "unitsuite.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("FString");
		addTest( Unit::compare1 );
		addTest( Unit::compare2 );
		addTest( Unit::appendSingle );
		addTest( Unit::shared1 );
	}

	virtual ~Unit()
	{
	}

	void compare1()
	{
		Bu::FString b("Bob");
		unitTest( !(b == "Bobo") );
		unitTest( b == "Bob" );
	}
	
	void compare2()
	{
		Bu::FString b("Bobo");
		unitTest( !(b == "Bob") );
		unitTest( b == "Bobo" );
	}

	void appendSingle()
	{
		Bu::FString b;
		for( char l = 'a'; l < 'g'; l++ )
			b += l;
		unitTest( b == "abcdef" );
		unitTest( strcmp( b.getStr(), "abcdef" ) == 0 );
	}

	void shared1()
	{
		Bu::FString a("Hey there");
		Bu::FString b( a );
		unitTest( a.getStr() == b.getStr() );
		b += " guy";
		unitTest( a.getStr() != b.getStr() );
		a = b;
		unitTest( a.getStr() == b.getStr() );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

