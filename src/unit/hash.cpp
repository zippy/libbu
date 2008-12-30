/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/unitsuite.h"

#include <stdio.h>

class Unit : public Bu::UnitSuite
{
private:
	typedef Bu::Hash<Bu::FString, int> StrIntHash;
	typedef Bu::Hash<Bu::FString, Bu::FString> StrStrHash;
	typedef Bu::Hash<int, Bu::FString> IntStrHash;

public:
	Unit()
	{
		setName("Hash");
		addTest( Unit::insert1 );
		addTest( Unit::insert2 );
		addTest( Unit::insert3 );
		addTest( Unit::probe1 );
		addTest( Unit::erase1 );
	}

	virtual ~Unit()
	{
	}

	void probe1()
	{
		StrIntHash h;
		char buf[20];
		for(int i=1;i<10000;i++)
		{
			sprintf(buf,"%d",i);
			Bu::FString sTmp(buf);
			h[sTmp] = i;
			unitTest( h.has(sTmp) );
		}
	}

	void insert1()
	{
		StrIntHash h;
		h["Hi"] = 42;
		unitTest( h["Hi"] == 42 );
	}

	void insert2()
	{
		StrStrHash h;
		h["Hi"] = "Yo";
		h["Bye"] = "Later";
		unitTest( h["Hi"].getValue() == "Yo" );

		StrStrHash h2(h);
		unitTest( h2["Hi"].getValue() = "Yo" );
		unitTest( h2["Bye"].getValue() = "Later" );

		StrStrHash h3;
		h3 = h;
		unitTest( h3["Hi"].getValue() = "Yo" );
		unitTest( h3["Bye"].getValue() = "Later" );
	}

	void insert3()
	{
		IntStrHash h;

		for( unsigned int i=1; i<50; i++ )
		{
			h[i] = "testing";
			unitTest( h.getSize() == i );
		}
	}

	void erase1()
	{
		StrIntHash h;
		h.insert("Number 1", 1 );
		h.insert("Number 2", 2 );
		h.insert("Number 3", 3 );
		h.erase("Number 2");
		h.get("Number 3");
		try {
			h.get("Number 2");
			unitFailed("h.get(\"Number 2\") should have thrown an exception.");
		} catch( Bu::HashException &e ) { }

		printf("\n");
		for( StrIntHash::iterator i = h.begin(); i != h.end(); i++ )
		{
			printf(" - \"%s\" = %d\n", i.getKey().getStr(), i.getValue() );
		}
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

