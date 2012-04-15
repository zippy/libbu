/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/myriadfs.h"
#include "bu/myriadstream.h"

#include <string.h>
#include <unistd.h>
#include <time.h>

#include "bu/sio.h"
using Bu::sio;
using Bu::Fmt;

namespace Bu { subExceptionDef( MyriadFsException ) }

#define Myriad_Fs_MAGIC_CODE		((char *)"\xa7\x18\x8b\x39")

Bu::MyriadFs::MyriadFs( Bu::Stream &rStore, int iBlockSize ) :
	rStore( rStore ),
	mStore( rStore, iBlockSize ),
	iUser( 0 ),
	iGroup( 0 )
{
#ifndef WIN32
	iUser = getuid();
	iGroup = getgid();
#endif

	if( mStore.hasStream( 1 ) )
	{
		// Check to see if this is a MyriadFs stream.
		Bu::MyriadStream ms = mStore.openStream( 1 );
		char sMagic[4];
		if( ms.read( sMagic, 4 ) < 4 )
			throw MyriadFsException("The provided stream does not appear to be "
				"a MyriadFs stream.");
		if( ::strncmp( sMagic, Myriad_Fs_MAGIC_CODE, 4 ) )
			throw MyriadFsException("The provided stream does not appear to be "
				"a MyriadFs stream.");
	
		int8_t iVer;
		ms.read( &iVer, 1 );

		int32_t iNumNodes;
		ms.read( &iNumNodes, 4 );
		for( int32_t j = 0; j < iNumNodes; j++ )
		{
			int32_t iNode;
			int32_t iPos;
			ms.read( &iNode, 4 );
			ms.read( &iPos, 4 );
			hNodeIndex.insert( iNode, iPos );
		}
	}
	else
	{
		// Create initial header stream
		{
			mStore.createStream( 1 );
			Bu::MyriadStream ms = mStore.openStream( 1 );
			ms.write( Myriad_Fs_MAGIC_CODE, 4 );
			int8_t iVer = 1;
			int32_t iTmp = 1;
			ms.write( &iVer, 1 );
			ms.write( &iTmp, 4 ); // iNumNodes
			iTmp = 0;
			ms.write( &iTmp, 4 ); // iInode
			ms.write( &iTmp, 4 ); // iPosition
			hNodeIndex.insert( 0, 0 );
		}

		// Create initial inode stream, with one root node.
		{
			mStore.createStream( 2 );
			Bu::MyriadStream ms = mStore.openStream( 2 );
			RawStat rs;
			rs.iNode = 0;
			rs.iUser = iUser;
			rs.iGroup = iGroup;
			rs.uPerms = 0755|typeDir;
			rs.iLinks = 1;
			rs.uStreamIndex = 3;
			rs.iCTime = rs.iMTime = rs.iATime = time(NULL);
			ms.write( &rs, sizeof(RawStat) );
		}

		// Create inode 0's storage stream.
		{
			mStore.createStream( 3 );
			Bu::MyriadStream ms = mStore.openStream( 3 );
			int32_t iTmp32 = 0;
			ms.write( &iTmp32, 4 );		// iChildCount
		}
	}
}

Bu::MyriadFs::~MyriadFs()
{
	writeHeader();
}

void Bu::MyriadFs::stat( const Bu::String &sPath, Bu::MyriadFs::Stat &rBuf )
{
	int32_t iParent;
	int32_t iNode = lookupInode( sPath, iParent );
	Bu::MyriadStream is = mStore.openStream( 2 );
	stat( iNode, rBuf, is );
}

Bu::MyriadStream Bu::MyriadFs::open( const Bu::String &sPath, int /*iMode*/,
		uint16_t uPerms )
{
	int32_t iParent = -1;
	int32_t iNode;
	try
	{
		iNode = lookupInode( sPath, iParent );
		sio << "File found." << sio.nl;
		// The file was found
		return openByInode( iNode );
	}
	catch( Bu::MyriadFsException &e )
	{
		if( iParent < 0 )
			throw;

		// This means that an intermediate path component couldn't be found
		if( e.getErrorCode() == 1 )
			throw;

		// The file wasn't found, but the path leading up to it was.
		// first, figure out the final path element...
		Bu::String sName = filePart( sPath );
		sio << "End filename: " << sName << sio.nl;
		sio << "Parent inode: " << iParent << sio.nl;
		iNode = create( iParent, sName, (uPerms&permMask)|typeRegFile, 0 );
		sio << "New iNode: " << iNode << sio.nl;
		return openByInode( iNode );
	}
}

void Bu::MyriadFs::create( const Bu::String &sPath, uint16_t iPerms )
{
	create( sPath, iPerms, 0 );
}

void Bu::MyriadFs::create( const Bu::String &sPath, uint16_t iPerms,
		uint16_t iDevHi, uint16_t iDevLo )
{
	create( sPath, iPerms, ((uint32_t)iDevHi<<16)|(uint32_t)iDevLo );
}

void Bu::MyriadFs::create( const Bu::String &sPath, uint16_t iPerms,
		uint32_t uSpecial )
{
	int32_t iParent = -1;
	int32_t iNode;
	try
	{
		iNode = lookupInode( sPath, iParent );
		sio << "File found." << sio.nl;
		// The file was found
		throw Bu::MyriadFsException("Path already exists.");
	}
	catch( Bu::MyriadFsException &e )
	{
		if( iParent < 0 )
			throw;

		// This means that an intermediate path component couldn't be found
		if( e.getErrorCode() == 1 )
			throw;

		// The file wasn't found, but the path leading up to it was.
		// first, figure out the final path element...
		Bu::String sName = filePart( sPath );
		sio << "End filename: " << sName << sio.nl;
		sio << "Parent inode: " << iParent << sio.nl;
		iNode = create( iParent, sName, iPerms, uSpecial );
		sio << "New iNode: " << iNode << sio.nl;
	}
}

void Bu::MyriadFs::mkDir( const Bu::String &sPath, uint16_t iPerms )
{
	create( sPath, (iPerms&permMask)|typeDir, 0 );
}

void Bu::MyriadFs::mkSymLink( const Bu::String &sTarget,
		const Bu::String &sPath )
{
	int32_t iParent = -1;
	int32_t iNode;
	try
	{
		iNode = lookupInode( sPath, iParent );
		throw Bu::MyriadFsException("Path already exists.");
	}
	catch( Bu::MyriadFsException &e )
	{
		if( iParent < 0 )
			throw;

		// This means that an intermediate path component couldn't be found
		if( e.getErrorCode() == 1 )
			throw;

		// The file wasn't found, but the path leading up to it was.
		// first, figure out the final path element...
		Bu::String sName = filePart( sPath );
		sio << "End filename: " << sName << sio.nl;
		sio << "Parent inode: " << iParent << sio.nl;
		iNode = create( iParent, sName, 0777|typeSymLink, 0 );
		sio << "New iNode: " << iNode << sio.nl;
		MyriadStream ms = openByInode( iNode );
		ms.write( sTarget );
	}
}

void Bu::MyriadFs::mkHardLink( const Bu::String &sTarget,
		const Bu::String &sPath )
{
	int32_t iParent = -1;
	int32_t iNode;
	
	iNode = lookupInode( sTarget, iParent );

	try
	{
		lookupInode( sPath, iParent );
		throw Bu::MyriadFsException("Path already exists.");
	}
	catch( Bu::MyriadFsException &e )
	{
		if( iParent < 0 )
			throw;

		// This means that an intermediate path component couldn't be found
		if( e.getErrorCode() == 1 )
			throw;

		// The file wasn't found, but the path leading up to it was.
		// first, figure out the final path element...
		Bu::String sName = filePart( sPath );
		sio << "End filename: " << sName << sio.nl;
		sio << "Parent inode: " << iParent << sio.nl;
		addToDir( iParent, iNode, sName );
		MyriadStream is = mStore.openStream( 2 );
		RawStat rs;
		readInode( iNode, rs, is );
		rs.iLinks++;
		writeInode( rs, is );
	}
}

Bu::String Bu::MyriadFs::readSymLink( const Bu::String &sPath )
{
	int32_t iParent = -1;
	int32_t iNode;
	iNode = lookupInode( sPath, iParent );
	MyriadStream ms = openByInode( iNode );
	Bu::String sRet;
	sRet.setSize( ms.getSize() );
	ms.read( sRet.getStr(), ms.getSize() );
	return sRet;
}

Bu::MyriadFs::Dir Bu::MyriadFs::readDir( const Bu::String &sPath )
{
	int32_t iParent = -1;
	int32_t iNode = lookupInode( sPath, iParent );
	return readDir( iNode );
}

void Bu::MyriadFs::setTimes( const Bu::String &sPath, int64_t iATime,
		int64_t iMTime )
{
	int32_t iParent = -1;
	int32_t iNode;

	iNode = lookupInode( sPath, iParent );
	
	setTimes( iNode, iATime, iMTime );
}

void Bu::MyriadFs::unlink( const Bu::String &sPath )
{
	int32_t iParent = -1;
//	int32_t iNode;

	/*iNode =*/ lookupInode( sPath, iParent );

	Dir lDir = readDir( iParent );

	Bu::String sName = filePart( sPath );

	for( Dir::iterator i = lDir.begin(); i; i++ )
	{
		if( sName == (*i).sName )
		{
			RawStat rs;
			readInode( (*i).iNode, rs );
			if( (rs.uPerms&typeMask) == typeDir )
			{
				MyriadStream msDir = mStore.openStream( rs.uStreamIndex );
				int32_t iCount;
				msDir.read( &iCount, 4 );
				if( iCount > 0 )
				{
					throw Bu::MyriadFsException("Directory not empty.");
				}
			}
			if( --rs.iLinks == 0 )
			{
				destroyNode( (*i).iNode );
			}
			else
			{
				writeInode( rs );
			}
			lDir.erase( i );
			break;
		}
	}
	
	Bu::MyriadStream ms = openByInode( iParent );
	int32_t iNumChildren = lDir.getSize();
	ms.write( &iNumChildren, 4 );
	for( Dir::iterator i = lDir.begin(); i; i++ )
	{
		ms.write( &(*i).iNode, 4 );
		uint8_t iSize = (*i).sName.getSize();
		ms.write( &iSize, 1 );
		ms.write( (*i).sName.getStr(), iSize );
	}
	ms.setSize( ms.tell() );
}

void Bu::MyriadFs::setFileSize( const Bu::String &sPath, int32_t iSize )
{
	int32_t iParent = -1;
	int32_t iNode;
	iNode = lookupInode( sPath, iParent );
	MyriadStream ms = openByInode( iNode );
	ms.setSize( iSize );
}

void Bu::MyriadFs::rename( const Bu::String &sFrom, const Bu::String &sTo )
{
	mkHardLink( sFrom, sTo );
	unlink( sFrom );
}

dev_t Bu::MyriadFs::devToSys( uint32_t uDev )
{
	return (((uDev&0xFFFF0000)>>8)&0xFF00) | ((uDev&0xFF));
}

uint32_t Bu::MyriadFs::sysToDev( dev_t uDev )
{
	return (((uint32_t)uDev&0xFF00)<<8) | ((uint32_t)uDev&0xFF);
}

int32_t Bu::MyriadFs::lookupInode( const Bu::String &sPath, int32_t &iParent )
{
	if( sPath == "/" )
	{
		return 0;
	}
	if( sPath[0] == '/' )
	{
		// Absolute lookup
		return lookupInode( sPath.begin()+1, 0, iParent );
	}
	else
	{
		// Relative lookup
		throw Bu::ExceptionBase(
			"Relative lookups in MyriadFs are not working yet.");
	}
}

int32_t Bu::MyriadFs::lookupInode( Bu::String::const_iterator iStart,
		int32_t iNode, int32_t &iParent )
{
	iParent = iNode;

	Bu::String::const_iterator iEnd = iStart.find('/');
	Bu::String sTok( iStart, iEnd );

//	sio << "Direcotry component: " << sTok << sio.nl;

	Dir lDir = readDir( iNode );

	for( Dir::iterator i = lDir.begin(); i; i++ )
	{
		if( (*i).sName == sTok )
		{
			// We found an item
			if( !iEnd )
			{
				// It's the last one in the requested path, return it
				return (*i).iNode;
			}
			else
			{
				// Not the last one in our path, double check it's a dir
				if( ((*i).uPerms&typeMask) == typeDir )
				{
					return lookupInode( iEnd+1, (*i).iNode, iParent );
				}
				else
				{
					iParent = -1;
					throw Bu::MyriadFsException(
						"Element '%s' in given path is not a directory.",
						sTok.getStr() );
				}
			}
		}
	}

	if( iEnd )
		throw Bu::MyriadFsException( 1, "Path not found");
	else
		throw Bu::MyriadFsException( 2, "Path not found");
}

void Bu::MyriadFs::readInode( int32_t iNode, RawStat &rs, MyriadStream &rIs )
{
	rIs.setPos( hNodeIndex.get( iNode )*sizeof(RawStat) );
	if( rIs.read( &rs, sizeof(RawStat) ) < sizeof(RawStat) )
		throw Bu::MyriadFsException("Filesystem corruption detected.");
	if( rs.iNode != iNode )
		throw Bu::MyriadFsException("Filesystem corruption detected.");
}

void Bu::MyriadFs::readInode( int32_t iNode, RawStat &rs )
{
	MyriadStream ms = mStore.openStream( 2 );
	readInode( iNode, rs, ms );
}

void Bu::MyriadFs::writeInode( const RawStat &rs,
		MyriadStream &rOs )
{
	rOs.setSize( hNodeIndex.getSize()*sizeof(RawStat) );
	rOs.setPos( hNodeIndex.get( rs.iNode )*sizeof(RawStat) );
	if( rOs.write( &rs, sizeof(RawStat) ) < sizeof(RawStat) )
		throw Bu::MyriadFsException("Error writing inode to header stream.");
}

void Bu::MyriadFs::writeInode( const RawStat &rs )
{
	MyriadStream ms = mStore.openStream( 2 );
	writeInode( rs, ms );
}

Bu::MyriadFs::Dir Bu::MyriadFs::readDir( int32_t iNode )
{
	Bu::MyriadStream ms = openByInode( iNode );
	int32_t iNumChildren = 0;
	ms.read( &iNumChildren, 4 );
	
	Bu::MyriadStream is = mStore.openStream( 2 );
	Dir lDir;
//	sio << "Reading dir, " << iNumChildren << " entries:" << sio.nl;
	for( int32_t j = 0; j < iNumChildren; j++ )
	{
		int32_t iChildNode;
		ms.read( &iChildNode, 4 );
		Stat s;
		stat( iChildNode, s, is );
		uint8_t uLen;
		ms.read( &uLen, 1 );
		s.sName.setSize( uLen );
		ms.read( s.sName.getStr(), uLen );
		lDir.append( s );

//		sio << "    " << s.sName << sio.nl;
	}

	return lDir;
}

Bu::MyriadStream Bu::MyriadFs::openByInode( int32_t iNode )
{
	RawStat rs;
	readInode( iNode, rs );
	switch( (rs.uPerms&typeMask) )
	{
		case typeDir:
		case typeSymLink:
		case typeRegFile:
			return mStore.openStream( rs.uStreamIndex );

		default:
			throw Bu::MyriadFsException(
				"inode incorrect type for low-level openByInode.");
	}
}

void Bu::MyriadFs::addToDir( int32_t iDir, int32_t iNode,
		const Bu::String &sName )
{
	if( sName.getSize() > 255 )
	{
		throw Bu::MyriadFsException("Filename too long, max is 255 bytes.");
	}
	Bu::MyriadStream ms = openByInode( iDir );
	int32_t iNumChildren = 0;
	ms.read( &iNumChildren, 4 );
	iNumChildren++;
	ms.setPos( 0 );
	ms.write( &iNumChildren, 4 );
	ms.setPosEnd( 0 );
	ms.write( &iNode, 4 );
	uint8_t uLen = sName.getSize();
	ms.write( &uLen, 1 );
	ms.write( sName.getStr(), uLen );
}

int32_t Bu::MyriadFs::create( int32_t iParent, const Bu::String &sName,
		uint16_t uPerms, uint32_t uSpecial )
{
	int32_t iNode = allocInode( uPerms, uSpecial );
	addToDir( iParent, iNode, sName );
	return iNode;
}

int32_t Bu::MyriadFs::allocInode( uint16_t uPerms, uint32_t uSpecial )
{
	int32_t iNode = 0;
	for(; iNode < 0xfffffff; iNode++ )
	{
		if( !hNodeIndex.has( iNode ) )
		{
			hNodeIndex.insert( iNode, hNodeIndex.getSize() );
			RawStat rs;
			rs.iNode = iNode;
			rs.iUser = iUser;
			rs.iGroup = iGroup;
			rs.uPerms = uPerms;
			rs.iLinks = 1;
			switch( (uPerms&typeMask) )
			{
				case typeRegFile:
				case typeSymLink:
					rs.uStreamIndex = mStore.createStream();
					break;
				
				case typeDir:
					rs.uStreamIndex = mStore.createStream();
					sio << "Creating directory node, storage: "
						<< rs.uStreamIndex << sio.nl;
					{
						Bu::MyriadStream msDir = mStore.openStream(
							rs.uStreamIndex
							);
						uint32_t uSize = 0;
						msDir.write( &uSize, 4 );
					}
					break;

				case typeChrDev:
				case typeBlkDev:
					rs.uStreamIndex = uSpecial;
					break;

				default:
					rs.uStreamIndex = 0;
					break;
			}
			rs.iATime = time(NULL);
			rs.iMTime = time(NULL);
			rs.iCTime = time(NULL);
			writeInode( rs );

			return iNode;
		}
	}

	throw Bu::MyriadFsException(
		"No inode could be allocated.  You've run out!");
}

void Bu::MyriadFs::stat( int32_t iNode, Stat &rBuf, MyriadStream &rIs )
{
	RawStat rs;
	readInode( iNode, rs, rIs );
	rBuf.iNode = iNode;
	rBuf.iUser = rs.iUser;
	rBuf.iGroup = rs.iGroup;
	rBuf.uPerms = rs.uPerms;
	rBuf.iLinks = rs.iLinks;
	rBuf.iATime = rs.iATime;
	rBuf.iMTime = rs.iMTime;
	rBuf.iCTime = rs.iCTime;
	rBuf.uDev = 0;
	rBuf.iSize = 0;
	switch( (rBuf.uPerms&typeMask) )
	{
		case typeRegFile:
		case typeSymLink:
			rBuf.iSize = mStore.getStreamSize( rs.uStreamIndex );
			break;

		case typeChrDev:
		case typeBlkDev:
			rBuf.uDev = rs.uStreamIndex;
			break;

		default:
			rBuf.iSize = 0;
			break;
	}
}

void Bu::MyriadFs::writeHeader()
{
	Bu::MyriadStream ms = mStore.openStream( 1 );
	ms.write( Myriad_Fs_MAGIC_CODE, 4 );
	int8_t iVer = 1;
	int32_t iNumNodes = hNodeIndex.getSize();
	ms.write( &iVer, 1 );
	ms.write( &iNumNodes, 4 ); // iNumNodes
	for( NodeIndex::iterator i = hNodeIndex.begin(); i; i++ )
	{
		int32_t iNode = i.getKey();
		int32_t iPosition = i.getValue();
		ms.write( &iNode, 4 );
		ms.write( &iPosition, 4 );
	}

	// Truncate the stream afterwards so we don't use up too much space.
	ms.setSize( ms.tell() );
}

void Bu::MyriadFs::setTimes( int32_t iNode, int64_t iATime, int64_t iMTime )
{
	RawStat rs;
	Bu::MyriadStream is = mStore.openStream( 2 );

	readInode( iNode, rs, is );
	rs.iATime = iATime;
	rs.iMTime = iMTime;
	writeInode( rs, is );
}

void Bu::MyriadFs::destroyNode( int32_t iNode )
{
	if( iNode == 0 )
		throw Bu::MyriadFsException("You cannot destroy the root.");

	Bu::MyriadStream is = mStore.openStream( 2 );

	// This will be overwritten with the last node
	uint32_t iPosition = hNodeIndex.get( iNode );
	RawStat rsOld;
	readInode( iNode, rsOld, is );
	switch( (rsOld.uPerms&typeMask) )
	{
		case typeRegFile:
		case typeDir:
		case typeSymLink:
			mStore.deleteStream( rsOld.uStreamIndex );
			break;
	}

	hNodeIndex.erase( iNode );

	// Read the last node, can't use the helpers, because we don't know the
	// iNode yet.
	if( iPosition != hNodeIndex.getSize() )
	{
		// If this is the last node, then we don't need to do anything, but
		// this case handles what to do if we aren't on the last node
		RawStat rs;
		is.setPos( (hNodeIndex.getSize())*sizeof(RawStat) );
		is.read( &rs, sizeof(RawStat) );

		hNodeIndex.get( rs.iNode ) = iPosition;
		writeInode( rs, is );
	}

	is.setSize( hNodeIndex.getSize() * sizeof(RawStat) );
}

Bu::String Bu::MyriadFs::filePart( const Bu::String &sPath )
{
	Bu::String::const_iterator iStart = sPath.begin();
	if( *iStart == '/' )
		iStart++;
	for( Bu::String::const_iterator iEnd = iStart.find('/'); iEnd;
		 iStart = iEnd+1, iEnd = iStart.find('/') ) { }
	return Bu::String( iStart, sPath.end() );
}

