/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
	iBlockStart( -1 ),
	iUsed( 0 )
{
}

Bu::Nids::~Nids()
{
	updateHeader();
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
	sStore.read( buf, 2 );
	if( buf[0] != 0 )
		throw NidsException(
			"We can only handle version 0 for now.");
	if( buf[1] != 4 )
		throw NidsException(
			"We can only handle 4-byte words at the moment.");
	sStore.read( &iBlockSize, 4 );
	sStore.read( &iBlocks, 4 );
	sStore.read( &iUsed, 4 ); // number of used blocks...
	sStore.seek( 7 ); // skip reserved space.
	iBlockStart = sStore.tell();

	//printf("%d blocks, %db each, %db block offset\n",
	//	iBlocks, iBlockSize, iBlockStart );

	bsBlockUsed.setSize( iBlocks, true );
	Block bTmp;
	for( int j = 0; j < iBlocks; j++ )
	{
		sStore.seek( iBlockStart+iBlockSize*j );
		sStore.read( &bTmp, sizeof(bTmp) );
		if( bTmp.uFirstBlock != blockUnused )
			bsBlockUsed.setBit( j );
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
	//printf("iBlockStart = %d\n", this->iBlockStart );
	bsBlockUsed.setSize( iPreAllocate, true );

	//printf("%d blocks, %db each, %db block offset\n",
	//	iBlocks, iBlockSize, iBlockStart );

	Block *block = (Block *)new char[iBlockSize];
	memset( block, 0, iBlockSize );
	block->uFirstBlock = block->uNextBlock = block->uPrevBlock = blockUnused;
	for( int j = 0; j < iPreAllocate; j++ )
	{
		sStore.write( block, iBlockSize );
	}
	delete[] (char *)block;
}

void Bu::Nids::updateHeader()
{
	if( !sStore.canWrite() )
		return;
	sStore.setPos( 10 ); // Skip the magic number, version, bpi, block size
	sStore.write( &iBlocks, 4 );
	sStore.write( &iUsed, 4 );
}

void Bu::Nids::initBlock( uint32_t uPos, uint32_t uFirstBlock,
	uint32_t uPrevBlock, bool bNew )
{
	Block b = { uPos, blockUnused, uPrevBlock, 0, 0, { } };
	if( uFirstBlock != blockUnused )
		b.uFirstBlock = uFirstBlock;
	bsBlockUsed.setBit( uPos );
	sStore.setPos( iBlockStart+(iBlockSize*uPos) );
	sStore.write( &b, sizeof(Block) );
	if( bNew )
	{
		// It's a new one, at the end, write some zeros.
		int iSize = iBlockSize-sizeof(Block);
		char *buf = new char[iSize];
		memset( buf, 0, iSize );
		sStore.write( buf, iSize );
		delete[] buf;
	}
	iUsed++;
}

uint32_t Bu::Nids::createBlock( uint32_t uFirstBlock, uint32_t uPrevBlock,
	int /*iPreAllocate*/ )
{
	for( int j = 0; j < iBlocks; j++ )
	{
		if( !bsBlockUsed.getBit( j ) )
		{
			initBlock( j, uFirstBlock, uPrevBlock );
			return j;
		}
	}
	// Oh, we don't have any blocks left...allocate a new one.
	iBlocks++;
	bsBlockUsed.setSize( iBlocks, false );
	initBlock( iBlocks-1, uFirstBlock, uPrevBlock, true );
	return iBlocks-1;
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

int Bu::Nids::getNumBlocks()
{
	return iBlocks;
}

int Bu::Nids::getNumUsedBlocks()
{
	return iUsed;
}

int Bu::Nids::getBlockStart()
{
	return iBlockStart;
}

int Bu::Nids::getBlockOverhead()
{
	return sizeof(Block);
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
	if( !sStore.canWrite() )
		return;
	sStore.setPos( iBlockStart + (iBlockSize*uIndex)+4*3 );
	sStore.write( &uSize, 4 );
}

uint32_t Bu::Nids::getNextBlock( uint32_t uIndex,
	struct Bu::Nids::Block *pBlock, bool bCreate )
{
	uint32_t uNew;
	if( pBlock->uNextBlock == blockUnused )
	{
		if( bCreate )
		{
			uNew = createBlock( pBlock->uFirstBlock, uIndex );
			sStore.setPos( iBlockStart + (iBlockSize*uIndex)+1*4 );
			sStore.write( &uNew, 4 );
			getBlock( uNew, pBlock );
			//printf("Allocated new block (%u) for stream %u.\n",
			//	uNew, pBlock->uFirstBlock );
		}
		else
		{
			throw Bu::NidsException("Reached end of stream.");
		}
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

