/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/file.h"
#include "bu/membuf.h"
#include "bu/base64.h"

int main( int argc, char *argv[] )
{
	argc--,argv++;

	if( argc < 3 )
		return 0;

	if( argv[0][0] == 'e' )
	{
		argv++;
		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::File fOut( argv[1], Bu::File::WriteNew );
		Bu::Base64 bOut( fOut );

		char buf[900];
		for(;;)
		{
			int iRead = fIn.read( buf, 900 );
			bOut.write( buf, iRead );
			if( iRead < 900 )
				break;
		}
	}
	else if( argv[0][0] == 'd' )
	{
		argv++;
		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::File fOut( argv[1], Bu::File::WriteNew );
		Bu::Base64 bIn( fIn );

		char buf[1024];
		for(;;)
		{
			int iRead = bIn.read( buf, 1024 );
			printf("Read %d bytes.\n", iRead );
			fOut.write( buf, iRead );
			if( iRead == 0 )
				break;
		}
	}
	else if( argv[0][0] == 'D' )
	{
		argv++;
		Bu::MemBuf mIn;
		{
			Bu::File fIn( argv[0], Bu::File::Read );
			char buf[1024];
			for(;;)
			{
				int iRead = fIn.read( buf, 1024 );
				mIn.write( buf, iRead );
				if( iRead < 1024 )
					break;
			}
			mIn.setPos( 0 );
		}
		Bu::File fOut( argv[1], Bu::File::WriteNew );
		Bu::Base64 bIn( mIn );

		char buf[1024];
		for(;;)
		{
			int iRead = bIn.read( buf, 1024 );
			printf("Read %d bytes.\n", iRead );
			fOut.write( buf, iRead );
			if( iRead == 0 )
				break;
		}
	}

	return 0;
}
