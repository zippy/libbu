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
	modeStreamDump,
	modeStreamPut,
	modeStreamGet,

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
		addOption( eMode, 'd', "dump",
				"Display a hexdump of a stream from a Myriad file.");
		addOption( eMode, "get",
				"Get a file out of a Myriad stream (use --dst).");
		addOption( eMode, "put",
				"Put a file into a Myriad stream (usr --src).");
		addHelpOption();

		addHelpBanner("\nGeneral options:");
		addOption( iBlockSize, 'b', "block-size", "Set the block size." );
		addOption( iPreallocate, 'p', "preallocate",
			"Number of blocks to preallocate." );
		addOption( sFile, 'f', "file", "Set the Myriad filename." );
		addOption( iStream, 's', "stream", "Substream to work with.");
		addOption( sSrc, "src", "Source file for copying into a Myriad file.");
		addOption( sDst, "dst",
				"Destination file for copying out of a Myriad file.");

		setOverride( "create", modeCreate );
		setOverride( "info", modeInfo );
		setOverride( "new", modeStreamNew );
		setOverride( "dump", modeStreamDump );
		setOverride( "put", modeStreamPut );
		setOverride( "get", modeStreamGet );

		parse( argc, argv );
	}

	Mode eMode;
	int iBlockSize;
	int iPreallocate;
	int iStream;
	Bu::String sFile;
	Bu::String sSrc;
	Bu::String sDst;
};

Bu::Formatter &operator>>( Bu::Formatter &f, Mode &e )
{
	sio << "Uh oh, the formatter was called..." << sio.nl;
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
				File fOut( opts.sFile, File::WriteNew|File::Read );
				Myriad m( fOut, opts.iBlockSize, opts.iPreallocate );
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
				sio << "Myriad info:" << sio.nl
					<< "  Block size:   " << m.getBlockSize() << sio.nl
					<< "  Block count:  " << m.getNumBlocks() << sio.nl
					<< "  Blocks used:  " << m.getNumUsedBlocks() << " ("
					<< m.getNumUsedBlocks()*100/m.getNumBlocks() << "%)"
					<< sio.nl
					<< "  Stream count: " << m.getNumStreams() << sio.nl
					<< "  Used space:   " << m.getTotalUsedBytes() << sio.nl
					<< "  Unused space: " << m.getTotalUnusedBytes() << sio.nl
					<< "  % of files:   " << (double)(m.getNumBlocks()*m.getBlockSize())/(double)(m.getTotalUsedBytes() + m.getTotalUnusedBytes( 4096 ))*100.0 << sio.nl;
				Bu::Array<int> aStreams = m.getStreamIds();
				sio << "  Stream info:" << sio.nl;
				for( Bu::Array<int>::iterator i = aStreams.begin(); i; i++ )
				{
					sio << "    " << Fmt(4) << *i << ") "
						<< m.getStreamSize( *i ) << "b" << sio.nl;
				}
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
				m.createStream( opts.iPreallocate );
			}
			break;

		case modeStreamDump:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file to manipulate." << sio.nl;
				return 0;
			}
			else
			{
				File fOut( opts.sFile, File::Read );
				Myriad m( fOut );
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
				sio << "Position: " << s.tell() << ", isEos()=" << s.isEos()
					<< sio.nl;
			}
			break;

		case modeStreamPut:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file manipulate." << sio.nl;
				return 0;
			}
			else if( !opts.sSrc.isSet() )
			{
				sio << "Please specify a source file to read." << sio.nl;
			}
			else
			{
				File fOut( opts.sFile, File::Write|File::Read );
				Myriad m( fOut );
				MyriadStream sOut = m.openStream(
					m.createStream( opts.iPreallocate )
					);
				File fIn( opts.sSrc, File::Read );
				char buf[1024];
				while( !fIn.isEos() )
				{
					sOut.write( buf, fIn.read( buf, 1024 ) );
				}
			}
			break;

		case modeStreamGet:
			if( !opts.sFile.isSet() )
			{
				sio << "Please specify a file manipulate." << sio.nl;
				return 0;
			}
			else if( !opts.sDst.isSet() )
			{
				sio << "Please specify a destination file to write." << sio.nl;
			}
			else
			{
				File fIn( opts.sFile, File::Write|File::Read );
				Myriad m( fIn );
				MyriadStream sIn = m.openStream( opts.iStream );
				File fOut( opts.sDst, File::Write|File::Create|File::Truncate );
				char buf[1024];
				while( !sIn.isEos() )
				{
					fOut.write( buf, sIn.read( buf, 1024 ) );
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

