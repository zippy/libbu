/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/file.h"
#include "bu/tafreader.h"

#include <string.h>
#include <unistd.h>

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("taf");
		addTest( Unit::read1 );
	}

	virtual ~Unit()
	{
	}

	void read1()
	{
#define FN_TMP ("/tmp/tmpXXXXXXXX")
		Bu::FString sFnTmp(FN_TMP);
		Bu::File fOut = Bu::File::tempFile( sFnTmp, "wb" );
		const char *data =
"{test: name=\"Bob\"}"
;
		fOut.write(data,strlen(data));
		fOut.close();

		Bu::File fIn(sFnTmp.getStr(), "rb");
		Bu::TafReader tr(fIn);
		
		Bu::TafGroup *tn = tr.readGroup();
		unitTest( !strcmp("Bob", tn->getProperty("name").getStr()) );
		delete tn;

		unlink(sFnTmp.getStr());
#undef FN_TMP
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

