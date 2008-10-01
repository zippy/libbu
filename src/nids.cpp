#include "bu/nids.h"
#include "bu/stream.h"
#include "bu/nidsstream.h"
#include <stdio.h>

#define NIDS_MAGIC_CODE		((unsigned char *)"\xFF\xC3\x99\xBD")

namespace Bu
{
	subExceptionDef( NidsException )
}

Bu::Nids::Nids( Bu::Stream &sStore ) :
	sStore( sStore ),
	iBlockSize( 0 ),
	iBlocks( 0 ),
	iBlockStart( -1 )
{
	printf("blockUnused = %u\n", blockUnused );
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
	printf("Magic:  %02X%02X%02X%02X\n",
		NIDS_MAGIC_CODE[0], NIDS_MAGIC_CODE[1],
		NIDS_MAGIC_CODE[2], NIDS_MAGIC_CODE[3]
		);
}

Bu::Nids::~Nids()
{
}

void Bu::Nids::initialize()
{
	unsigned char buf[4];
	if( sStore.read( buf, 4 ) < 4 )
		throw NidsException("Input stream appears to be empty.");
	if( memcmp( buf, NIDS_MAGIC_CODE, 4 ) )
	{
		throw NidsException(
			"Stream does not appear to be a valid NIDS format.");
	}


}

void Bu::Nids::initialize( int iBlockSize, int iPreAllocate )
{
	char cBuf = 0;
	int iBuf = 0;

	// Magic number
	sStore.write( NIDS_MAGIC_CODE, 4 );

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
	printf("iBlockStart = %d\n", this->iBlockStart );
	bsBlockUsed.setSize( iPreAllocate, true );

	printf("%d blocks, %db each, %db block offset\n",
		iBlocks, iBlockSize, iBlockStart );

	Block *block = (Block *)new char[iBlockSize];
	memset( block, 0, iBlockSize );
	block->uFirstBlock = block->uNextBlock = block->uPrevBlock = blockUnused;
	for( int j = 0; j < iPreAllocate; j++ )
	{
		sStore.write( block, iBlockSize );
	}
	delete[] (char *)block;
}

uint32_t Bu::Nids::createBlock( uint32_t uFirstBlock, uint32_t uPrevBlock,
	int /*iPreAllocate*/ )
{
	for( int j = 0; j < iBlocks; j++ )
	{
		if( !bsBlockUsed.getBit( j ) )
		{
			Block b = { j, blockUnused, uPrevBlock, 0, 0, { } };
			if( uFirstBlock != blockUnused )
				b.uFirstBlock = uFirstBlock;
			bsBlockUsed.setBit( j );
			sStore.setPos( iBlockStart+(iBlockSize*j) );
			sStore.write( &b, sizeof(b) );
			return j;
		}
	}
	return blockUnused;
}

int Bu::Nids::createStream( int iPreAllocate )
{
	return createBlock( blockUnused, blockUnused, iPreAllocate );
}

void Bu::Nids::deleteStream( int /*iID*/ )
{
}

Bu::NidsStream Bu::Nids::openStream( int iID )
{
	if( iBlockStart < 0 )
	{
		initialize();
	}
	return NidsStream( *this, iID );
}

int Bu::Nids::getBlockSize()
{
	return iBlockSize;
}
/*
void Bu::Nids::extendStream( int iID, int iBlockCount )
{
}*/

void Bu::Nids::getBlock( uint32_t uIndex, Bu::Nids::Block *pBlock )
{
	sStore.setPos( iBlockStart + (iBlockSize*uIndex) );
	sStore.read( pBlock, iBlockSize );
}

void Bu::Nids::setBlock( uint32_t uIndex, Bu::Nids::Block *pBlock )
{
	sStore.setPos( iBlockStart + (iBlockSize*uIndex) );
	sStore.write( pBlock, iBlockSize );
}

void Bu::Nids::updateStreamSize( uint32_t uIndex, uint32_t uSize )
{
	sStore.setPos( iBlockStart + (iBlockSize*uIndex)+4*3 );
	sStore.write( &uSize, 4 );
}

uint32_t Bu::Nids::getNextBlock( uint32_t uIndex,
	struct Bu::Nids::Block *pBlock )
{
	uint32_t uNew;
	if( pBlock->uNextBlock == blockUnused )
	{
		uNew = createBlock( pBlock->uFirstBlock, uIndex );
		sStore.setPos( iBlockStart + (iBlockSize*uIndex)+1*4 );
		sStore.write( &uNew, 4 );
		getBlock( uNew, pBlock );
	}
	else
	{
		uNew = pBlock->uNextBlock;
		getBlock( pBlock->uNextBlock, pBlock );
	}
	return uNew;
}

Bu::Nids::Block *Bu::Nids::newBlock()
{
	return (Block *)new char[iBlockSize];
}

void Bu::Nids::deleteBlock( Block *pBlock )
{
	delete[] (char *)pBlock;
}

