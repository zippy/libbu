/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/fstring.h"
#include "bu/unitsuite.h"
#include "bu/xmlreader.h"
#include "bu/membuf.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("Xml");
		addTest( Unit::declaration );
	}

	virtual ~Unit()
	{
	}

	void declaration()
	{
		Bu::FString sXml("<?xml ?> <hi />");
		Bu::MemBuf buf( sXml );
		Bu::XmlReader xr( buf );
	}
	
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

