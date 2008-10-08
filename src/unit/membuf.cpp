/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/membuf.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("MemBuf");
		addTest( Unit::testWriteRead01 );
		addTest( Unit::testOverwrite1 );
	}

	virtual ~Unit()
	{
	}

	void testWriteRead01()
	{
		Bu::MemBuf mb;
		unitTest( mb.write("ab", 2 ) == 2 );
		unitTest( mb.write("cde", 3 ) == 3 );
		unitTest( mb.write("FG", 2 ) == 2 );

		mb.setPos( 0 );

		char buf[8];
		buf[7] = '\0';
		unitTest( mb.read( buf, 7 ) == 7 );
		unitTest( !strncmp( buf, "abcdeFG", 7 ) );
		unitTest( mb.read( buf, 7 ) == 0 );
		mb.seek( -3 );
		unitTest( mb.read( buf, 7 ) == 3 );
		unitTest( !strncmp( buf, "eFG", 3 ) );
	}

	void testOverwrite1()
	{
		Bu::MemBuf mb;
		unitTest( mb.write("0123456789") == 10 );
		mb.setPos( 4 );
		unitTest( mb.write("-5-") == 3 );
		mb.setPos( 9 );
		mb.write("Hey!!!");
		unitTest( mb.tell() == 15 );
		char buf[50];
		mb.setPos( 0 );
		buf[mb.read( buf, 50 )] = '\0';
		unitTest( !strcmp( buf, "0123-5-78Hey!!!" ) );
	}
};

int main( int argc, char *argv[] ){ return Unit().run( argc, argv ); }
