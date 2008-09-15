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

public:
	Unit()
	{
		setName("Hash");
		addTest( Unit::insert1 );
		addTest( Unit::insert2 );
		addTest( Unit::probe1 );
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
		unitTest( h["Hi"].getValue() == "Yo" );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

