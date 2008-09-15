#include "bu/nids.h"
#include "bu/stream.h"
#include "bu/nidsstream.h"
#include <stdio.h>

Bu::Nids::Nids( Bu::Stream &sStore ) :
	sStore( sStore )
{
	printf("Stream caps:\n"
		"  canRead:    %s\n"
		"  canWrite:   %s\n"
		"  isReadable: %s\n"
		"  isWritable: %s\n"
		"  isSeekable: %s\n"
		"  isBlocking: %s\n"
		"  isEOS:      %s\n"
		"  isOpen:     %s\n",
		sStore.canRead()?"yes":"no",
		sStore.canWrite()?"yes":"no",
		sStore.isReadable()?"yes":"no",
		sStore.isWritable()?"yes":"no",
		sStore.isSeekable()?"yes":"no",
		sStore.isBlocking()?"yes":"no",
		sStore.isEOS()?"yes":"no",
		sStore.isOpen()?"yes":"no"
		);
	printf("sizeof(Block) = %db\n", sizeof(Block) );
}

Bu::Nids::~Nids()
{
}

void Bu::Nids::initialize( int iBlockSize, int iPreAllocate )
{
	char cBuf = 0;
	int iBuf = 0;

	// Magic number
	sStore.write( "\xFF\xC3\x99\xBD", 4 );

	// Version (0)
	sStore.write( &cBuf, 1 );

	// Bytes per int
	cBuf = 4;
	sStore.write( &cBuf, 1 );

	// The size of each block and the number of blocks we're pre-allocating
	sStore.write( &iBlockSize, 4 );
	sStore.write( &iPreAllocate, 4 );

	// The number of used blocks
	sStore.write( &iBuf, 4 );

	// Reserved space
	sStore.write( "\x00\x00\x00\x00\x00\x00\x00", 7 );

	this->iBlockSize = iBlockSize;
	this->iBlocks = iPreAllocate;
	this->iBlockStart = sStore.tell();
	bsBlockUsed.setSize( iPreAllocate, true );

	printf("%d blocks, %db each, %db block offset\n",
		iBlocks, iBlockSize, iBlockStart );

	char *block = new char[iBlockSize];
	memset( block, 0, iBlockSize );
	for( int j = 0; j < iPreAllocate; j++ )
	{
		sStore.write( block, iBlockSize );
	}
}

int Bu::Nids::createStream( int iPreAllocate )
{
	for( int j = 0; j < iBlocks; j++ )
	{
		if( !bsBlockUsed.getBit( j ) )
		{
			Block b = { j, blockUnused, blockUnused, 0, 0 };
			bsBlockUsed.setBit( j );
			sStore.setPos( iBlockStart+(iBlockSize*j) );
			sStore.write( &b, sizeof(b) );
		}
	}
	return 0;
}

void Bu::Nids::deleteStream( int iID )
{
}

Bu::NidsStream Bu::Nids::openStream( int iID )
{
	return NidsStream( *this );
}

void Bu::Nids::extendStream( int iID, int iBlockCount )
{
}

void Bu::Nids::getBlock( int iIndex, Bu::Nids::Block *pBlock )
{
	sStore.setPos( iBlockStart + (iBlockSize*iIndex) );
	sStore.read( pBlock, iBlockSize );
}

void Bu::Nids::setBlock( int iIndex, Bu::Nids::Block *pBlock )
{
	sStore.setPos( iBlockStart + (iBlockSize*iIndex) );
	sStore.write( pBlock, iBlockSize );
}

