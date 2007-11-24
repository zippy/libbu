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
		addTest( Unit::insert );
		addTest( Unit::remove );
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

	void insert()
	{
		Bu::FString a("abcd");
		a.insert( 2, "-!-", 3 );
		unitTest( a == "ab-!-cd" );
		
		a.insert( 0, "!!", 2 );
		unitTest( a == "!!ab-!-cd" );
		
		a.insert( -10, "789", 3 );
		unitTest( a == "789!!ab-!-cd" );
		
		a.insert( 12, "89", 2 );
		unitTest( a == "789!!ab-!-cd89" );
		
		a.insert( 1203, "12", 2 );
		unitTest( a == "789!!ab-!-cd8912" );
	}

	void remove()
	{
		Bu::FString a("abHEYcd");
		a.remove( 2, 3 );
		unitTest( a == "abcd" );
		a.remove( 2, 5 );
		unitTest( a == "ab" );
		a += "cdefghijklmnop";
		a.remove( 5, 1 );
		unitTest( a = "abcdeghijklmnop" );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

