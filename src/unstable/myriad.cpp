/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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

#define Myriad_MAGIC_CODE		((unsigned char *)"\x0a\xd3\xfa\x84")

namespace Bu
{
	subExceptionDef( MyriadException )
	template<typename t> t blkDiv( t total, t block ) {
		return (total/block)+((total%block==0)?(0):(1));
	}
}

Bu::Myriad::Myriad( Bu::Stream &sStore, int iBlockSize, int iPreallocate ) :
	sStore( sStore ),
	iBlockSize( iBlockSize ),
	iBlocks( 0 ),
	iUsed( 0 ),
	bHeaderChanged( false )
{
	try
	{
		initialize();
	}
	catch( Bu::MyriadException &e )
	{
		if( e.getErrorCode() == MyriadException::emptyStream )
		{
			initialize( iBlockSize, iPreallocate );
		}
		else
		{
			throw;
		}
	}
}

Bu::Myriad::~Myriad()
{
	if( !hActiveBlocks.isEmpty() )
	{
		sio << "Bu::Myriad::~Myriad(): Error: There are "
			<< hActiveBlocks.getSize() << " unsynced blocks!" << sio.nl;
	}
	sync();

	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		delete *i;
	}
}

void Bu::Myriad::sync()
{
	updateHeader();

	for( BlockHash::iterator i = hActiveBlocks.begin(); i; i++ )
	{
		if( (*i)->bChanged )
		{
			syncBlock( *i );
		}
	}
}

void Bu::Myriad::initialize()
{
	sStore.setPosEnd( 0 );
	int iSize = sStore.tell();
	sStore.setPos( 0 );

	unsigned char buf[4];
	if( sStore.read( buf, 4 ) < 4 )
		throw MyriadException( MyriadException::emptyStream,
				"Input stream appears to be empty.");
	if( memcmp( buf, Myriad_MAGIC_CODE, 4 ) )
	{
		throw MyriadException( MyriadException::invalidFormat,
			"Stream does not appear to be a valid Myriad format.");
	}
	sStore.read( buf, 2 );
	if( buf[0] != 1 )
		throw MyriadException( MyriadException::badVersion,
			"We can only handle version 1 for now.");
	if( buf[1] != 32 )
		throw MyriadException( MyriadException::invalidWordSize,
			"We can only handle 32-bit words at the moment.");
	sStore.read( &iBlockSize, 4 );
	int iStreams;
	sStore.read( &iStreams, 4 );

	iBlocks = iSize/iBlockSize;
	//sio << "Myriad: iSize=" << iSize << ", iBlockSize=" << iBlockSize
	//	<< ", iBlocks=" << iBlocks << ", iStreams=" << iStreams << sio.nl;

	int iHeaderSize = 14 + 8 + 4;
	int iHeaderBlocks = 0; //blkDiv( iHeaderSize+4, iBlockSize );

	while( iHeaderSize > iHeaderBlocks*iBlockSize )
	{
		iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );
		iHeaderSize = 14 + 8 + 4*iHeaderBlocks;
	}

	//sio << "Myriad: iHeaderSize=" << iHeaderSize
	//	<< ", iHeaderBlocks=" << iHeaderBlocks << sio.nl;

	Stream *pFakeHdr = new Stream;
	pFakeHdr->iId = 0;
	pFakeHdr->iSize = iHeaderSize;
	for( int j = 0; j < iHeaderBlocks; j++ )
	{
		pFakeHdr->aBlocks.append( j );
	}
	
	bsBlockUsed.setSize( iBlocks, true );

//	bool bCanSkip = false; // Can skip around, post initial header stream i/o
	MyriadStream *pIn = new MyriadStream( *this, pFakeHdr );
	pIn->setPos( sStore.tell() );
	for( int j = 0; j < iStreams; j++ )
	{
		aStreams.append( new Stream() );
		Stream &s = *aStreams[j];
		pIn->read( &s.iId, 4 );
		pIn->read( &s.iSize, 4 );
		int iSBlocks = blkDiv(s.iSize, iBlockSize);
	//	sio << "Myriad: - Stream::iId=" << s.iId
	//		<< ", Stream::iSize=" << s.iSize
	//		<< ", Stream::aBlocks=" << iSBlocks
	//		<< ", pIn->tell()=" << pIn->tell() << sio.nl;
		for( int k = 0; k < iSBlocks; k++ )
		{
			int iBId;
			pIn->read( &iBId, 4 );
	//		sio << "Myriad:   - iBId=" << iBId
	//			<< ", iStartPos=" << iBId*iBlockSize
	//			<< ", pIn->tell()=" << pIn->tell() << sio.nl;
			s.aBlocks.append( iBId );
			bsBlockUsed.setBit( iBId );
			iUsed++;
			if( (j == 0 && k == iHeaderBlocks-1) )
			{
	//			sio << "Myriad:   - End of prepartition, unlocking skipping."
	//				<< sio.nl;
//				bCanSkip = true;
				MyriadStream *pTmp = new MyriadStream( *this, aStreams[0] );
	//			sio << "Myriad    - Position = " << pIn->tell() << sio.nl;
				pTmp->setPos( pIn->tell() );
				delete pIn;
				delete pFakeHdr;
				pIn = pTmp;
			}
		}
	}
	delete pIn;

	//sio << "Myriad: Blocks used: " << bsBlockUsed.toString() << sio.nl;
}

void Bu::Myriad::initialize( int iBlockSize, int iPreAllocate )
{
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		delete *i;
	}
	aStreams.clear();
	iUsed = 0;

	int iHeaderSize = 14 + 8 + 4;
	int iHeaderBlocks = 0; //blkDiv( iHeaderSize+4, iBlockSize );
	char cBuf = 1;
	int iBuf = 0;
	
	Stream *pStr = new Stream;
	pStr->iId = 0;

	while( iHeaderSize > iHeaderBlocks*iBlockSize )
	{
		iHeaderBlocks = blkDiv( iHeaderSize+4, iBlockSize );
		iHeaderSize = 14 + 8 + 4*iHeaderBlocks;
	}

	iPreAllocate += iHeaderBlocks;

	//sio << "Myriad: iHeaderSize=" << iHeaderSize << ", iBlockSize="
	//	<< iBlockSize << ", iHeaderBlocks=" << iHeaderBlocks << sio.nl;
	
	bsBlockUsed.setSize( iPreAllocate, true );
	iUsed++;

	char *pBlock = new char[iBlockSize];
	memset( pBlock, 0, iBlockSize );
	for( int j = 0; j < iPreAllocate; j++ )
	{
		sStore.write( pBlock, iBlockSize );
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
//	sio << "Myriad: Actual end of header stream = " << pStr->iSize << sio.nl;

	pStr->iSize = iHeaderSize;
	for( int j = 0; j < iHeaderBlocks; j++ )
	{
		pStr->aBlocks.append( j );
		bsBlockUsed.setBit( j );
		iUsed++;
	}

	aStreams.append( pStr );

	//sio << bsBlockUsed.toString() << " - " << pStr->aBlocks << sio.nl;

	bHeaderChanged = true;
	//hStreams.insert( 0, BlockArray( 0 ) );
}

void Bu::Myriad::updateHeader()
{
	if( bHeaderChanged == false )
		return;
	if( !sStore.canWrite() )
		return;

	char cBuf;
	int iBuf;

	//for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	//{
	//	sio << "Myriad: Stream " << Fmt(4) << (*i)->iId << ": " << (*i)->aBlocks << sio.nl;
	//}

	// Compute the new size of the header.
	int iHeaderSize = 14 + 8*aStreams.getSize();
//	sio << "Myriad: updateHeader: aStreams.getSize() = " << aStreams.getSize()
//		<< sio.nl;
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		iHeaderSize += 4*(*i)->aBlocks.getSize();
//		sio << "Myriad: updateHeader: (*i)->aBlocks.getSize() = "
//			<< (*i)->aBlocks.getSize() << sio.nl;
	}
	int iNewBlocks = blkDiv( iHeaderSize, iBlockSize );
	while( iNewBlocks > aStreams[0]->aBlocks.getSize() )
	{
		int iBlock = findEmptyBlock();
//		sio << "Myriad: updateHeader: Appending block " << iBlock
//			<< " to header." << sio.nl;
		aStreams[0]->aBlocks.append( iBlock );
		bsBlockUsed.setBit( iBlock );
		iUsed++;
		iHeaderSize += 4;
		iNewBlocks = blkDiv( iHeaderSize, iBlockSize );
	}
	aStreams[0]->iSize = iHeaderSize;
//	sio << "Myriad: updateHeader: iHeaderSize=" << iHeaderSize
//		<< ", iNewBlocks=" << iNewBlocks << ", curBlocks="
//		<< aStreams[0]->aBlocks.getSize() << sio.nl;

	MyriadStream sHdr( *this, aStreams[0] );
	sHdr.write( Myriad_MAGIC_CODE, 4 );

	// Version (1)
	cBuf = 1;
	sHdr.write( &cBuf, 1 );

	// Bits per int
	cBuf = 32;
	sHdr.write( &cBuf, 1 );

	// The size of each block
	sHdr.write( &iBlockSize, 4 );

	iBuf = aStreams.getSize();
	// The number of streams
	sHdr.write( &iBuf, 4 );
	
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		sHdr.write( &(*i)->iId, 4 );
		sHdr.write( &(*i)->iSize, 4 );
		int iUsedBlocks = blkDiv( (*i)->iSize, iBlockSize );
//		for( BlockArray::iterator j = (*i)->aBlocks.begin(); j; j++ )
		for( int j = 0; j < iUsedBlocks; j++ )
		{
			sHdr.write( &(*i)->aBlocks[j], 4 );
		}
	}
	
	bHeaderChanged = false;
}

int Bu::Myriad::createStream( int iPreAllocate )
{
	Stream *pStr = new Stream();
	pStr->iId = aStreams.last()->iId+1;
	//sio << "Myriad: New stream id=" << pStr->iId << ", iPreAllocate="
	//	<< iPreAllocate << sio.nl;
	pStr->iSize = 0;
	aStreams.append( pStr );

	for( int j = 0; j < iPreAllocate; j++ )
	{
		int iFreeBlock = findEmptyBlock();
//		sio << "Myriad: Adding block " << iFreeBlock << sio.nl;
		pStr->aBlocks.append( iFreeBlock );
		bsBlockUsed.setBit( iFreeBlock );
		iUsed++;
	}

	bHeaderChanged = true;

	return pStr->iId;
}

int Bu::Myriad::createStreamWithId( int iId, int iPreAllocate )
{
	try
	{
		findStream( iId );
		throw MyriadException( MyriadException::streamExists,
			"There is already a stream with the given id.");
	}
	catch( MyriadException &e )
	{
		Stream *pStr = new Stream();
		pStr->iId = iId;
		//sio << "Myriad: New stream id=" << pStr->iId << ", iPreAllocate="
		//	<< iPreAllocate << sio.nl;
		pStr->iSize = 0;
		if( aStreams.last()->iId < iId )
		{
			aStreams.append( pStr );
		}
		else
		{
			for( StreamArray::iterator i = aStreams.begin(); i; i++ )
			{
				if( (*i)->iId > iId )
				{
					aStreams.insert( i, pStr );
					break;
				}
			}
		}

		for( int j = 0; j < iPreAllocate; j++ )
		{
			int iFreeBlock = findEmptyBlock();
	//		sio << "Myriad: Adding block " << iFreeBlock << sio.nl;
			pStr->aBlocks.append( iFreeBlock );
			bsBlockUsed.setBit( iFreeBlock );
			iUsed++;
		}

		bHeaderChanged = true;

		return pStr->iId;
	}
}

int Bu::Myriad::findEmptyBlock()
{
	bHeaderChanged = true;

	for( int j = 0; j < bsBlockUsed.getSize(); j++ )
	{
		if( bsBlockUsed.getBit( j ) == false )
			return j;
	}
//	sio << "Myriad: findEmptyBlock(): No empty blocks, adding new one." << sio.nl;

	bsBlockUsed.setSize( bsBlockUsed.getSize()+1, false );
	/*
	sStore.setPos( iBlockSize*iBlocks );

	char *pBlock = new char[iBlockSize];
	memset( pBlock, 0, iBlockSize );
	sStore.write( pBlock, iBlockSize );
	delete[] pBlock;
	*/

	sStore.setSize( (iBlocks+1)*iBlockSize );

	return iBlocks++;
}

void Bu::Myriad::deleteStream( int iId )
{
	if( iId < 0 )
		throw MyriadException( MyriadException::invalidStreamId,
				"Invalid stream id.");
	if( iId == 0 )
		throw MyriadException( MyriadException::protectedStream,
				"You cannot delete stream zero, it is protected.");
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		if( (*i)->iId == iId )
		{
			Stream *pStream = *i;
			for( BlockArray::iterator j = pStream->aBlocks.begin(); j; j++ )
			{
				bsBlockUsed.setBit( *j, false );
				iUsed--;
			}
			aStreams.erase( i );
			bHeaderChanged = true;
			delete pStream;
			return;
		}
	}
}

Bu::Array<int> Bu::Myriad::getStreamIds()
{
	Bu::Array<int> aRet( aStreams.getSize() );
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		aRet.append( (*i)->iId );
	}

	return aRet;
}

int Bu::Myriad::getStreamSize( int iId )
{
	return findStream( iId )->iSize;
}

bool Bu::Myriad::hasStream( int iId )
{
	try
	{
		findStream( iId );
		return true;
	}catch(...)
	{
		return false;
	}
}

Bu::MyriadStream Bu::Myriad::openStream( int iId )
{
	//sio << "Myriad:  Request to open stream: " << iId << sio.nl;
	return MyriadStream( *this, findStream( iId ) );
}

int Bu::Myriad::getNumStreams()
{
	return aStreams.getSize();
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

int Bu::Myriad::getTotalUsedBytes()
{
	int iTotalSize = 0;
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		iTotalSize += (*i)->iSize;
	}
	return iTotalSize;
}

int Bu::Myriad::getTotalUnusedBytes()
{
	int iTotalSize = (iBlocks-iUsed)*iBlockSize;
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		iTotalSize += iBlockSize - ((*i)->iSize%iBlockSize);
	}
	return iTotalSize;
}

int Bu::Myriad::getTotalUnusedBytes( int iFakeBlockSize )
{
	int iTotalSize = (iBlocks-iUsed)*iFakeBlockSize;
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		iTotalSize += iFakeBlockSize - ((*i)->iSize%iFakeBlockSize);
	}
	return iTotalSize;
}

Bu::Myriad::Stream *Bu::Myriad::findStream( int iId )
{
	for( StreamArray::iterator i = aStreams.begin(); i; i++ )
	{
		if( (*i)->iId == iId )
			return *i;
	}

	throw MyriadException( MyriadException::noSuchStream,
		"The requested stream doesn't exist and cannot be opened." );

	return NULL;
}

Bu::Myriad::Block *Bu::Myriad::getBlock( int iBlock )
{
//	sio << "Myriad:  Reading block " << iBlock << ", bytes "
//		<< iBlockSize*iBlock << "-" << iBlockSize*(iBlock+1) << sio.nl;
	Block *pBlock = new Block;
	pBlock->pData = new char[iBlockSize];
	sStore.setPos( iBlockSize * iBlock );
	sStore.read( pBlock->pData, iBlockSize );
	pBlock->bChanged = false;
	pBlock->iBlockIndex = iBlock;

	hActiveBlocks.insert( iBlock, pBlock );

	return pBlock;
}

void Bu::Myriad::releaseBlock( Bu::Myriad::Block *pBlock )
{
	if( pBlock == NULL )
		return;
//	sio << "Myriad:  Releasing block " << pBlock->iBlockIndex << sio.nl;
	syncBlock( pBlock );
	hActiveBlocks.erase( pBlock->iBlockIndex );
	delete[] pBlock->pData;
	delete pBlock;
}

void Bu::Myriad::syncBlock( Block *pBlock )
{
	if( pBlock->bChanged )
	{
//		sio << "Myriad:    - Block changed, writing back to stream." << sio.nl;
		sStore.setPos( iBlockSize * pBlock->iBlockIndex );
		sStore.write( pBlock->pData, iBlockSize );
		pBlock->bChanged = false;
	}
}

int Bu::Myriad::streamAddBlock( Stream *pStream )
{
	int iBlock = findEmptyBlock();
	pStream->aBlocks.append( iBlock );
	bsBlockUsed.setBit( iBlock );
	bHeaderChanged = true;
	iUsed++;
	return iBlock;
}

void Bu::Myriad::setStreamSize( Stream *pStream, long iSize )
{
	if( pStream->iSize == iSize )
	{
		return;
	}
	else if( pStream->iSize > iSize )
	{
		// Shrink
		for( int iNewSize = pStream->aBlocks.getSize()*iBlockSize;
			 iNewSize-iBlockSize > iSize; iNewSize -= iBlockSize )
		{
			if( bsBlockUsed.getBit( pStream->aBlocks.last() ) )
				iUsed--;
			bsBlockUsed.setBit( pStream->aBlocks.last(), false );
			pStream->aBlocks.eraseLast();
		}
		pStream->iSize = iSize;
		bHeaderChanged = true;
	}
	else
	{
		// Grow
		for( int iNewSize = pStream->aBlocks.getSize()*iBlockSize;
			 iNewSize < iSize; iNewSize += iBlockSize )
		{
			streamAddBlock( pStream );
		}
		pStream->iSize = iSize;
		bHeaderChanged = true;
	}
}

void Bu::Myriad::headerChanged()
{
	bHeaderChanged = true;
}

bool Bu::Myriad::isMyriad( Bu::Stream &sStore )
{
	sStore.setPos( 0 );

	unsigned char buf[4];
	if( sStore.read( buf, 4 ) < 4 )
		throw MyriadException( MyriadException::emptyStream,
				"Input stream appears to be empty.");
	sStore.setPos( 0 );
	if( memcmp( buf, Myriad_MAGIC_CODE, 4 ) )
	{
		return false;
	}
	return true;
}

const Bu::BitString &Bu::Myriad::getBlocksUsed() const
{
	return bsBlockUsed;
}
