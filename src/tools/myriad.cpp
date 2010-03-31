/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"
#include "bu/file.h"
#include "bu/myriad.h"
#include "bu/myriadstream.h"
#include "bu/optparser.h"

#include <stdlib.h>

using namespace Bu;

enum Mode
{
	modeCreate,
	modeInfo,

	modeNone
};

class Options : public OptParser
{
public:
	Options( int argc, char *argv[] ) :
		eMode( modeNone ),
		iBlockSize( 64 ),
		iPreallocate( 0 )
	{
		addHelpBanner("Mode of operation:");
		addOption( eMode, 'c', "create", "Create a new NIDS file." );
		addOption( eMode, "info", "Display some info about a NIDS file." );
		addHelpOption();

		addHelpBanner("\nGeneral options:");
		addOption( iBlockSize, 'b', "block-size", "Set the block size." );
		addOption( iPreallocate, 'p', "preallocate",
			"Number of blocks to preallocate." );
		addOption( sOutput, 'o', "output", "Set the output filename." );
		addOption( sInput, 'i', "input", "Set the input filename." );

		setOverride( "create", "create" );
		setOverride( "info", "info" );

		parse( argc, argv );
	}

	Mode eMode;
	int iBlockSize;
	int iPreallocate;
	Bu::FString sOutput;
	Bu::FString sInput;
};

Bu::Formatter &operator>>( Bu::Formatter &f, Mode &m )
{
	Bu::FString sTok = f.readToken();
	if( sTok == "create" || sTok == "c" )
		m = modeCreate;
	else if( sTok == "info" )
		m = modeInfo;
	else
		m = modeNone;
	return f;
}

int main( int argc, char *argv[] )
{
	Options opts( argc, argv );

	switch( opts.eMode )
	{
		case modeCreate:
			if( !opts.sOutput.isSet() )
			{
				sio << "Please specify an output file to create a stream for."
					<< sio.nl;
				return 0;
			}
			else
			{
				File fOut( opts.sOutput, File::WriteNew );
				Myriad n( fOut );
				n.initialize( opts.iBlockSize, opts.iPreallocate );
			}
			break;

		case modeInfo:
			if( !opts.sInput.isSet() )
			{
				sio << "Please specify an input file to display info about."
					<< sio.nl;
				return 0;
			}
			else
			{
				File fIn( opts.sInput, File::Read );
				Myriad n( fIn );
				n.initialize();
			}
			break;

		case modeNone:
			sio << "Please select a mode, for more info, try --help."
				<< sio.nl << sio.nl;
			break;
	}

	return 0;
}

