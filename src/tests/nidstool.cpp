#include "bu/file.h"
#include "bu/nids.h"
#include "bu/nidsstream.h"
#include "bu/paramproc.h"

#include <stdlib.h>

class Param : public Bu::ParamProc
{
public:
	Param( int argc, char *argv[] )
	{
		addHelpBanner("nidstool - Do stuff with nids files.\n\n");
		addParam("info", 'i', mkproc(Param::procInfo),
			"Print some info about the file.");
		addParam("dump", 'd', mkproc(Param::procDump),
			"Dump a stream to a file");
		addParam("help", 'h', mkproc(Bu::ParamProc::help), "This help.");
		process( argc, argv );
	}

	virtual ~Param()
	{
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

		printf("Block size:     %db\n", n.getBlockSize() );
		printf("Block count:    %d\n", n.getNumBlocks() );
		printf("Blocks used:    %d (%d%%)\n", n.getNumUsedBlocks(),
			n.getNumUsedBlocks()*100/n.getNumBlocks() );
		printf("Block start:    %db\n", n.getBlockStart() );
		printf("Block overhead: %db\n", n.getBlockOverhead() );
		printf("Block storage:  %db (%d%%)\n",
			n.getBlockSize()-n.getBlockOverhead(),
			(n.getBlockSize()-n.getBlockOverhead())*100/n.getBlockSize() );

		if( argc >= 2 )
		{
			typedef struct Block
			{
				uint32_t uFirstBlock;
				uint32_t uNextBlock;
				uint32_t uPrevBlock;
				uint32_t uBytesUsed;
				uint32_t uReserved;
			} Block;

			uint32_t uStream = strtoul( argv[1], NULL, 0 );
			uint32_t uBlock = uStream;

			Block b;

			for(;;)
			{
				fIn.setPos( n.getBlockStart()+n.getBlockSize()*uBlock );
				fIn.read( &b, sizeof(Block) );
				printf("Stream %u:  block %u, next %u, prev %u, %ub used.\n",
					uStream, uBlock, b.uNextBlock, b.uPrevBlock, b.uBytesUsed
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

};


int main( int argc, char *argv[] )
{
	Param p( argc, argv );

	return 0;
}

