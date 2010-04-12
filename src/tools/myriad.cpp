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
	modeStreamNew,
	modeStreamRead,

	modeNone
};

class Options : public OptParser
{
public:
	Options( int argc, char *argv[] ) :
		eMode( modeNone ),
		iBlockSize( 64 ),
		iPreallocate( 0 ),
		iStream( 0 )
	{
		addHelpBanner("Mode of operation:");
		addOption( eMode, 'c', "create",
				"Create a new Myriad file." );
		addOption( eMode, 'i', "info",
				"Display some info about a Myriad file." );
		addOption( eMode, 'n', "new",
				"Create a new sub-stream in a Myriad file.");
		addOption( eMode, 'r', "read",
				"Read a stream from a Myriad file.");
		addHelpOption();

		addHelpBanner("\nGeneral options:");
		addOption( iBlockSize, 'b', "block-size", "Set the block size." );
		addOption( iPreallocate, 'p', "preallocate",
			"Number of blocks to preallocate." );
		addOption( sFile, 'f', "file", "Set the Myriad filename." );
		addOption( iStream, 's', "stream", "Substream to work with.");
		addOption( sSrc, "src", "Source file for copying into a Myriad file.");

		setOverride( "create", "create" );
		setOverride( "info", "info" );
		setOverride( "new", "new" );
		setOverride( "read", "read" );

		parse( argc, argv );
	}

	Mode eMode;
	int iBlockSize;
	int iPreallocate;
	int iStream;
	Bu::FString sFile;
	Bu::FString sSrc;
};

Bu::Formatter &operator>>( Bu::Formatter &f, Mode &m )
{
	Bu::FString sTok = f.readToken();
	if( sTok == "create" )
		m = modeCreate;
	else if( sTok == "info" )
		m = modeInfo;
	else if( sTok == "new" )
		m = modeStreamNew;
	else if( sTok == "read" )
		m = modeStreamRead;
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
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file to create." << sio.nl;
				return 0;
			}
			else
			{
				File fOut( opts.sFile, File::WriteNew );
				Myriad m( fOut );
				m.initialize( opts.iBlockSize, opts.iPreallocate );
			}
			break;

		case modeInfo:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file to display info about." << sio.nl;
				return 0;
			}
			else
			{
				File fIn( opts.sFile, File::Read );
				Myriad m( fIn );
				m.initialize();
			}
			break;

		case modeStreamNew:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file manipulate." << sio.nl;
				return 0;
			}
			else
			{
				File fOut( opts.sFile, File::Write|File::Read );
				Myriad m( fOut );
				m.initialize();
				m.createStream( opts.iPreallocate );
			}
			break;

		case modeStreamRead:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file manipulate." << sio.nl;
				return 0;
			}
			else
			{
				File fOut( opts.sFile, File::Read );
				Myriad m( fOut );
				m.initialize();
				MyriadStream s = m.openStream( opts.iStream );
				sio << "Stream " << opts.iStream << ":" << sio.nl;
				char buf[8];
				int iPos = 0;
				while( !s.isEos() )
				{
					size_t sAmnt = s.read( buf, 8 );
					sio << Fmt(5) << iPos << ": ";
					iPos += sAmnt;
					for( size_t j = 0; j < sAmnt; j++ )
					{
						sio << Fmt::hex(2) << (int)((unsigned char)buf[j])
							<< " ";
					}
					for( size_t j = sAmnt; j < 8; j++ )
					{
						sio << "-- ";
					}
					sio << "| ";
					for( size_t j = 0; j < sAmnt; j++ )
					{
						if( buf[j] >= 32 && buf[j] <= 126 )
							sio << buf[j] << " ";
						else
							sio << "  ";
					}
					sio << sio.nl;
				}
			}
			break;

		case modeNone:
			sio << "Please select a mode, for more info, try --help."
				<< sio.nl << sio.nl;
			break;
	}

	return 0;
}

