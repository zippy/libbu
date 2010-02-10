/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/myriad.h"
#include "bu/stream.h"
#include "bu/myriadstream.h"
#include <stdio.h>

#include "bu/sio.h"
using Bu::sio;
using Bu::Fmt;

#define Myriad_MAGIC_CODE		((unsigned char *)"\xFF\xC3\x99\xBD")

namespace Bu
{
	subExceptionDef( MyriadException )
	template<typename t> t blkDiv( t total, t block ) {
		return (total/block)+((total%block==0)?(0):(1));
	}
}

Bu::Myriad::Myriad( Bu::Stream &sStore ) :
	sStore( sStore ),
	iBlockSize( 0 ),
	iBlocks( 0 ),
	iUsed( 0 )
{
}

Bu::Myriad::~Myriad()
{
	updateHeader();

	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		delete *i;
	}
}

void Bu::Myriad::sync()
{
	updateHeader();

	// Later, also flush all caches.
}

void Bu::Myriad::initialize()
{
	sStore.setPosEnd( 0 );
	int iSize = sStore.tell();
	sStore.setPos( 0 );

	unsigned char buf[4];
	if( sStore.read( buf, 4 ) < 4 )
		throw MyriadException("Input stream appears to be empty.");
	if( memcmp( buf, Myriad_MAGIC_CODE, 4 ) )
	{
		throw MyriadException(
			"Stream does not appear to be a valid Myriad format.");
	}
	sStore.read( buf, 2 );
	if( buf[0] != 1 )
		throw MyriadException(
			"We can only handle version 1 for now.");
	if( buf[1] != 32 )
		throw MyriadException(
			"We can only handle 32-bit words at the moment.");
	sStore.read( &iBlockSize, 4 );
	int iStreams;
	sStore.read( &iStreams, 4 );

	iBlocks = iSize/iBlockSize;
	sio << "Myriad: iSize=" << iSize << ", iBlockSize=" << iBlockSize
		<< ", iBlocks=" << iBlocks << ", iStreams=" << iStreams << sio.nl;
	
	int iHeaderSize = 14 + 8 + 4;
	int iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );

	while( iHeaderSize > iHeaderBlocks*iBlockSize )
	{
		iHeaderSize = 14 + 8 + 4*iHeaderBlocks;
		iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );
	}

	sio << "Myriad: iHeaderSize=" << iHeaderSize
		<< ", iHeaderBlocks=" << iHeaderBlocks << sio.nl;
	
	bsBlockUsed.setSize( iBlocks, true );

	bool bCanSkip = false; // Can skip around, post initial header stream i/o
	for( int j = 0; j < iStreams; j++ )
	{
		int iHdrBlock = 0;
		int iCurBlock = 0;
		aStreams.append( new Stream() );
		Stream &s = *aStreams[j];
		sStore.read( &s.iId, 4 );
		sStore.read( &s.iSize, 4 );
		int iSBlocks = blkDiv(s.iSize, iBlockSize);
		sio << "Myriad: - Stream::iId=" << s.iId << ", Stream::iSize=" << s.iSize
			<< ", Stream::aBlocks=" << iSBlocks
			<< ", sStore.tell()=" << sStore.tell() << sio.nl;
		for( int k = 0; k < iSBlocks; k++ )
		{
			int iBId;
			sStore.read( &iBId, 4 );
			sio << "Myriad:   - iBId=" << iBId
				<< ", iStartPos=" << iBId*iBlockSize
				<< ", sStore.tell()=" << sStore.tell() << sio.nl;
			s.aBlocks.append( iBId );
			bsBlockUsed.setBit( iBId );
			if( (j == 0 && k == iHeaderBlocks-1) )
			{
				sio << "Myriad:   - End of prepartition, unlocking skipping."
					<< sio.nl;
				bCanSkip = true;
				iCurBlock = blkDiv( (int)sStore.tell(), iBlockSize );
			}
			if( bCanSkip && sStore.tell() >= iCurBlock*iBlockSize+iBlockSize )
			{
				iHdrBlock++;
				iCurBlock = aStreams[0]->aBlocks[iHdrBlock];
				sio << "Myriad: Ran out of data in block, finding next header "
					"block:  " << iHdrBlock << " = " << iCurBlock << " ("
					<< iCurBlock*iBlockSize << "b)" << sio.nl;
				sStore.setPos( iCurBlock*iBlockSize );
			}
		}
	}

	sio << bsBlockUsed.toString() << sio.nl;

	//printf("%d blocks, %db each, %db block offset\n",
	//	iBlocks, iBlockSize, iBlockStart );

}

void Bu::Myriad::initialize( int iBlockSize, int iPreAllocate )
{
	int iHeaderSize = 14 + 8 + 4;
	int iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );
	char cBuf = 1;
	int iBuf = 0;
	
	Stream *pStr = new Stream;
	pStr->iId = 0;

	while( iHeaderSize > iHeaderBlocks*iBlockSize )
	{
		iHeaderSize = 14 + 8 + 4*iHeaderBlocks;
		iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );
	}

	iPreAllocate += iHeaderBlocks;

	sio << "Myriad:  iHeaderSize=" << iHeaderSize << ", iBlockSize="
		<< iBlockSize << ", iHeaderBlocks=" << iHeaderBlocks << sio.nl;
	
	bsBlockUsed.setSize( iPreAllocate, true );

	char *pBlock = new char[iBlockSize];
	memset( pBlock, 0, iBlockSize );
	for( int j = 0; j < iPreAllocate; j++ )
	{
		sStore.write( pBlock, iBlockSize );
		pStr->aBlocks.append( j );
	}
	delete[] (char *)pBlock;

	sStore.setPos( 0 );

	// Magic number
	sStore.write( Myriad_MAGIC_CODE, 4 );

	// Version (0)
	sStore.write( &cBuf, 1 );

	// Bits per int
	cBuf = 32;
	sStore.write( &cBuf, 1 );

	// The size of each block
	sStore.write( &iBlockSize, 4 );

	iBuf = 1;
	// The number of streams
	sStore.write( &iBuf, 4 );
	
	// Stream header
	iBuf = 0;
	sStore.write( &iBuf, 4 );
	sStore.write( &iHeaderSize, 4 );
	for( iBuf = 0; iBuf < iHeaderBlocks; iBuf++ )
	{
		sStore.write( &iBuf, 4 );
	}

	this->iBlockSize = iBlockSize;
	this->iBlocks = iPreAllocate;
	
	pStr->iSize = sStore.tell();

	//hStreams.insert( 0, BlockArray( 0 ) );
}

void Bu::Myriad::updateHeader()
{
	if( !sStore.canWrite() )
		return;

	

	// TODO: Use the stream class to access this really smoothly, I hope :)
}

int Bu::Myriad::createStream( int iPreAllocate )
{
	Stream *pStr = new Stream();
	pStr->iId = aStreams.last()->iId+1;
	sio << "Myriad: New stream id=" << pStr->iId << ", iPreAllocate="
		<< iPreAllocate << sio.nl;
	pStr->iSize = 0;

	for( int j = 0; j < iPreAllocate; j++ )
	{
		int iFreeBlock = findEmptyBlock();
		sio << "Myriad: Adding block " << j << sio.nl;
		pStr->aBlocks.append( j );
		bsBlockUsed.setBit( j );
	}

	return 0;
}

int Bu::Myriad::findEmptyBlock()
{
	for( int j = 0; j < bsBlockUsed.getSize(); j++ )
	{
		if( bsBlockUsed.getBit( j ) == false )
			return j;
	}
	sio << "Myriad: findEmptyBlock(): No empty blocks, adding new one." << sio.nl;

	bsBlockUsed.setSize( bsBlockUsed.getSize()+1, false );
	sStore.setPos( iBlockSize*iBlocks );

	char *pBlock = new char[iBlockSize];
	memset( pBlock, 0, iBlockSize );
	sStore.write( pBlock, iBlockSize );
	delete pBlock;

	return iBlockSize++;
}

void Bu::Myriad::deleteStream( int /*iID*/ )
{
}

Bu::MyriadStream Bu::Myriad::openStream( int iID )
{
	return MyriadStream( *this, iID );
}

int Bu::Myriad::getBlockSize()
{
	return iBlockSize;
}

int Bu::Myriad::getNumBlocks()
{
	return iBlocks;
}

int Bu::Myriad::getNumUsedBlocks()
{
	return iUsed;
}


