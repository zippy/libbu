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
		setName("Archive");
		addTest( Unit::testPrimitives );
	}

	virtual ~Unit()
	{
	}

	void testPrimitives()
	{
		Bu::MemBuf mb;
		{
			Bu::Archive ar( mb, Bu::Archive::save );
			ar << (int8_t)1;
			ar << (uint8_t)2;
			ar << (int16_t)3;
			ar << (uint16_t)4;
			ar << (int32_t)5;
			ar << (uint32_t)6;
			ar << (int64_t)7;
			ar << (uint64_t)8;
			ar << (char)9;
			ar << (unsigned char)10;
			ar << (short)11;
			ar << (unsigned short)12;
			ar << (int)13;
			ar << (unsigned int)14;
			ar << (long)15;
			ar << (unsigned long)16;
			//ar << (long long)17;
			//ar << (unsigned long long)18;
			ar.close();
		}
		mb.setPos( 0 );
		{
			Bu::Archive ar( mb, Bu::Archive::load );
			int8_t p1;
			uint8_t p2;
			int16_t p3;
			uint16_t p4;
			int32_t p5;
			uint32_t p6;
			int64_t p7;
			uint64_t p8;
			signed char p9;
			unsigned char p10;
			short p11;
			unsigned short p12;
			int p13;
			unsigned int p14;
			long p15;
			unsigned long p16;
			//long long p17;
			//unsigned long long p18;
			ar >> p1;
			ar >> p2;
			ar >> p3;
			ar >> p4;
			ar >> p5;
			ar >> p6;
			ar >> p7;
			ar >> p8;
			ar >> p9;
			ar >> p10;
			ar >> p11;
			ar >> p12;
			ar >> p13;
			ar >> p14;
			ar >> p15;
			ar >> p16;
			//ar >> p17;
			//ar >> p18;
			unitTest( p1 == 1 );
			unitTest( p2 == 2 );
			unitTest( p3 == 3 );
			unitTest( p4 == 4 );
			unitTest( p5 == 5 );
			unitTest( p6 == 6 );
			unitTest( p7 == 7 );
			unitTest( p8 == 8 );
			unitTest( p9 == 9 );
			unitTest( p10 == 10 );
			unitTest( p11 == 11 );
			unitTest( p12 == 12 );
			unitTest( p13 == 13 );
			unitTest( p14 == 14 );
			unitTest( p15 == 15 );
			unitTest( p16 == 16 );
			//unitTest( p17 == 17 );
			//unitTest( p18 == 18 );
			ar.close();
		}
	}
};

int main( int argc, char *argv[] ){ return Unit().run( argc, argv ); }
