/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/file.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"
#include "bu/paramproc.h"

#include <stdlib.h>

typedef struct Block
{
	uint32_t uFirstBlock;
	uint32_t uNextBlock;
	uint32_t uBytesUsed;
} Block;

class Param : public Bu::ParamProc
{
public:
	Param( int argc, char *argv[] )
	{
		addHelpBanner("nidstool - Do stuff with nids files.\n\n");
		addParam("info", 'i', mkproc(Param::procInfo),
			"Print some info about the file.");
		addParam("dump", 'd', mkproc(Param::procDump),
			"Dump a stream to a file.");
		addParam("analyze", 'a', mkproc(Param::procAnalyze),
			"Analyze a nids file.");
		addParam("copy", 'c', mkproc(Param::procCopy),
			"Copy a nids file, changing settings.");
		addParam("help", 'h', mkproc(Bu::ParamProc::help), "This help.");
		process( argc, argv );
	}

	virtual ~Param()
	{
	}

	void printInfo( Bu::Nids &n )
	{
		printf("File info:\n");
		printf("  Header overhead: %db\n", n.getBlockStart() );
		printf("  Block size:      %db\n", n.getBlockSize() );
		printf("  Block count:     %d\n", n.getNumBlocks() );
		printf("  Blocks used:     %d (%d%%)\n", n.getNumUsedBlocks(),
			n.getNumUsedBlocks()*100/n.getNumBlocks() );
		printf("  Block overhead:  %db\n", n.getBlockOverhead() );
		printf("  Block storage:   %db (%d%%)\n",
			n.getBlockSize()-n.getBlockOverhead(),
			(n.getBlockSize()-n.getBlockOverhead())*100/n.getBlockSize() );
	}

	int procInfo( int argc, char *argv[] )
	{
		if( argc < 1 )
		{
			printf("You must provide a file name.\n");
			exit( 1 );
		}

		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::Nids n( fIn );
		n.initialize();

		printInfo( n );

		if( argc >= 2 )
		{
			uint32_t uStream = strtoul( argv[1], NULL, 0 );
			uint32_t uBlock = uStream;

			Block b;

			for(;;)
			{
				fIn.setPos( n.getBlockStart()+n.getBlockSize()*uBlock );
				fIn.read( &b, sizeof(Block) );
				printf("Stream %u:  block %u, next %u, %ub used.\n",
					uStream, uBlock, b.uNextBlock, b.uBytesUsed
					);
				if( b.uNextBlock == 0xFFFFFFFFUL )
					break;
				uBlock = b.uNextBlock;
			}
			printf("Stream End.\n");

			return 2;
		}

		return 1;
	}

	int procDump( int argc, char *argv[] )
	{
		if( argc < 3 )
		{
			printf("You must provide a nids file, a stream id, and an output "
				"file.\n");
			exit( 1 );
		}

		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::Nids n( fIn );
		n.initialize();

		int iStream = strtol( argv[1], NULL, 0 );
		Bu::NidsStream sIn = n.openStream( iStream );

		Bu::File fOut( argv[2], Bu::File::Write|Bu::File::Create );
		int iTotal = 0;
		char buf[100];
		for(;;)
		{
			int iRead = sIn.read( buf, 100 );
			iTotal += fOut.write( buf, iRead );
			if( iRead < 100 )
				break;
		}

		printf("Wrote %db from stream %d in %s to %s.\n",
			iTotal, iStream, argv[0], argv[2] );
		return 3;
	}

	int procAnalyze( int argc, char *argv[] )
	{
		if( argc < 1 )
		{
			printf("You must provide a file name.\n");
			exit( 1 );
		}

		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::Nids n( fIn );
		n.initialize();

		printInfo( n );

		int iStreamCnt = 0;
		int iStreamTotal = 0;
		int iOneBlock = 0;
		uint32_t iLargest = 0;
		uint32_t iSmallest = 0;
		int iWaste = 0;
		int iUsable = n.getBlockSize()-n.getBlockOverhead();
		Block b;
		for( int j = 0; j < n.getNumBlocks(); j++ )
		{
			fIn.setPos( n.getBlockStart()+n.getBlockSize()*j );
			fIn.read( &b, sizeof(Block) );
			if( b.uFirstBlock != (uint32_t)j )
				continue;
			
			iStreamCnt++;
			iStreamTotal += b.uBytesUsed;

			if( b.uNextBlock == 0xFFFFFFFFUL )
			{
				iOneBlock++;
				iWaste += iUsable - b.uBytesUsed;
			}
			else
			{
				iWaste += iUsable - (b.uBytesUsed%iUsable);
			}
			
			if( j == 0 )
			{
				iSmallest = iLargest = b.uBytesUsed;
			}
			else
			{
				if( iLargest < b.uBytesUsed )
					iLargest = b.uBytesUsed;
				if( iSmallest > b.uBytesUsed )
					iSmallest = b.uBytesUsed;
			}
		}
		printf("Steam analysis:\n");
		printf("  Stream count:          %d\n", iStreamCnt );
		printf("  Stream size:           %db/%db/%db (min/avr/max)\n",
			iSmallest, iStreamTotal/iStreamCnt, iLargest );
		printf("  One-block streams:     %d (%d%%)\n",
			iOneBlock, iOneBlock*100/iStreamCnt );
		printf("  Total wasted space:    %db (%d%%)\n",
			iWaste, iWaste*100/iStreamTotal );
		printf("  Avr blocks-per-stream: %f%%\n",
			(float)n.getNumBlocks()/(float)iStreamCnt );

		return 1;
	}

	int procCopy( int argc, char *argv[] )
	{
		if( argc < 3 )
		{
			printf("You must provide source stream, blocksize, destination.\n");
			exit( 1 );
		}

		Bu::File fIn( argv[0], Bu::File::Read );
		Bu::Nids nIn( fIn );
		nIn.initialize();

		Bu::File fOut( argv[2], Bu::File::Read|Bu::File::Write|Bu::File::Create|
				Bu::File::Truncate );
		Bu::Nids nOut( fOut );
		nOut.initialize( strtol( argv[1], 0, NULL ) );

		Block b;
		for( int j = 0; j < nIn.getNumBlocks(); j++ )
		{
			fIn.setPos( nIn.getBlockStart()+nIn.getBlockSize()*j );
			fIn.read( &b, sizeof(Block) );
			if( b.uFirstBlock != (uint32_t)j )
				continue;

			Bu::NidsStream sIn = nIn.openStream( j );
			int iNew = nOut.createStream();
			Bu::NidsStream sOut = nOut.openStream( iNew );

			char buf[1024];
			for(;;)
			{
				int iRead = sIn.read( buf, 1024 );
				sOut.write( buf, iRead );
				if( iRead < 1024 )
					break;
			}
		}

		return 3;
	}
};


int main( int argc, char *argv[] )
{
	Param p( argc, argv );

	return 0;
}

