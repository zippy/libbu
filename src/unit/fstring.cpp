/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/fstring.h"
#include "bu/unitsuite.h"

#include <dirent.h>

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
		addTest( Unit::add1 );
		addTest( Unit::add2 );
		addTest( Unit::add3 );
		addTest( Unit::add4 );
		addTest( Unit::add5 );
		addTest( Unit::add6 );
		addTest( Unit::subStr1 );
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

	void add1()
	{
		Bu::FString a("hi there");
		Bu::FString b(", yeah!");
		Bu::FString c = a + b;

		unitTest( c == "hi there, yeah!" );
	}

	void add2()
	{
		Bu::FString a("hi there");
		Bu::FString c = a + ", yeah!";

		unitTest( c == "hi there, yeah!" );
	}

	void add3()
	{
		Bu::FString a("hi there");
		Bu::FString b(", yeah!");
		Bu::FString c = a + ", Mr. Man" + b;

		unitTest( c == "hi there, Mr. Man, yeah!" );
	}

	void add4()
	{
		Bu::FString b(", yeah!");
		Bu::FString c = "hi there" + b;

		unitTest( c == "hi there, yeah!" );
	}

	void add5()
	{
		Bu::FString b;
		Bu::FString c = "sup?";
		b += "hey, " + c;
		
		unitTest( b == "hey, sup?" );
	}

	void add6()
	{
		Bu::FString a("Hello");
		char b[256] = {"Dude"};
		Bu::FString c = a + "/" + b;

		unitTest( c == "Hello/Dude" );
	}

	void subStr1()
	{
		Bu::FString a("abcdefghijklmnop");
		unitTest( a.getSubStr( 5, 3 ) == "fgh" );
		unitTest( a.getSubStr( 10 ) == "klmnop" );
		unitTest( a.getSubStr( 40 ) == "" );
		unitTest( a.getSubStr( -10 ) == "abcdefghijklmnop" );
		unitTest( a.getSubStr( -15, 4 ) == "abcd" );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

