/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/unitsuite.h"
#include "bu/file.h"
#include "bu/tafreader.h"
#include "bu/tafwriter.h"
#include "bu/membuf.h"

#include <string.h>
#include <unistd.h>

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("taf");
		addTest( Unit::read1 );
		addTest( Unit::encode );
	}

	virtual ~Unit()
	{
	}

	void read1()
	{
#define FN_TMP ("/tmp/tmpXXXXXXXX")
		Bu::FString sFnTmp(FN_TMP);
		Bu::File fOut = Bu::File::tempFile( sFnTmp );
		const char *data =
"{test: name=\"Bob\"}"
;
		fOut.write(data,strlen(data));
		fOut.close();

		Bu::File fIn(sFnTmp.getStr(), Bu::File::Read );
		Bu::TafReader tr(fIn);
		
		Bu::TafGroup *tn = tr.readGroup();
		unitTest( !strcmp("Bob", tn->getProperty("name").getStr()) );
		delete tn;

		unlink(sFnTmp.getStr());
#undef FN_TMP
	}

	void encode()
	{
		Bu::MemBuf mb;
		Bu::TafWriter tw( mb );
		
		Bu::TafGroup g("Test data");
		Bu::FString sData( 256 );
		for( int j = 0; j < 256; j++ )
			sData[j] = (unsigned char)j;
		g.addChild( new Bu::TafProperty("Encoded", sData) );
		tw.writeGroup( &g );

		static const char *cmpdata = "{\"Test data\":\n    \"Encoded\"=\""
			"\\x00\\x01\\x02\\x03\\x04\\x05\\x06\\x07"
			"\\x08\\x09\\x0A\\x0B\\x0C\\x0D\\x0E\\x0F"
			"\\x10\\x11\\x12\\x13\\x14\\x15\\x16\\x17"
			"\\x18\\x19\\x1A\\x1B\\x1C\\x1D\\x1E\\x1F"
			" !\\\"#$%&'()*+,-./0123456789:;<=>?@ABCD"
			"EFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghi"
			"jklmnopqrstuvwxyz{|}~\\x7F"
			"\\x80\\x81\\x82\\x83\\x84\\x85\\x86\\x87"
			"\\x88\\x89\\x8A\\x8B\\x8C\\x8D\\x8E\\x8F"
			"\\x90\\x91\\x92\\x93\\x94\\x95\\x96\\x97"
			"\\x98\\x99\\x9A\\x9B\\x9C\\x9D\\x9E\\x9F"
			"\\xA0\\xA1\\xA2\\xA3\\xA4\\xA5\\xA6\\xA7"
			"\\xA8\\xA9\\xAA\\xAB\\xAC\\xAD\\xAE\\xAF"
			"\\xB0\\xB1\\xB2\\xB3\\xB4\\xB5\\xB6\\xB7"
			"\\xB8\\xB9\\xBA\\xBB\\xBC\\xBD\\xBE\\xBF"
			"\\xC0\\xC1\\xC2\\xC3\\xC4\\xC5\\xC6\\xC7"
			"\\xC8\\xC9\\xCA\\xCB\\xCC\\xCD\\xCE\\xCF"
			"\\xD0\\xD1\\xD2\\xD3\\xD4\\xD5\\xD6\\xD7"
			"\\xD8\\xD9\\xDA\\xDB\\xDC\\xDD\\xDE\\xDF"
			"\\xE0\\xE1\\xE2\\xE3\\xE4\\xE5\\xE6\\xE7"
			"\\xE8\\xE9\\xEA\\xEB\\xEC\\xED\\xEE\\xEF"
			"\\xF0\\xF1\\xF2\\xF3\\xF4\\xF5\\xF6\\xF7"
			"\\xF8\\xF9\\xFA\\xFB\\xFC\\xFD\\xFE\\xFF\"\n}\n";
		unitTest( mb.getString() == cmpdata );
		mb.setPos( 0 );
		Bu::TafReader tr( mb );
		Bu::TafGroup *rg = tr.readGroup();
		unitTest( rg->getProperty("Encoded") == sData );
		delete rg;
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}

