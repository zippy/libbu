/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
		throw MyriadFsException("You totally have a MyriadFs stream, version %d, but they can't be loaded yet.", iVer );
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
			int32_t iTmp32 = 0;
			int16_t iTmp16 = 0;
			uint16_t uPerms = 775|typeDir;
			ms.write( &iUser, 4 );		// User
			ms.write( &iGroup, 4 );		// Group
			ms.write( &uPerms, 2 );		// Permissions/types
			ms.write( &iTmp16, 2 );		// Link count
			iTmp32 = 3;
			ms.write( &iTmp32, 4 );		// Stream index
			iTmp32 = 0;
			ms.write( &iTmp32, 4 );		// Parent inode (root's it's own parent)
			int64_t iTime = time(NULL);
			ms.write( &iTime, 8 );		// atime
			ms.write( &iTime, 8 );		// mtime
			ms.write( &iTime, 8 );		// ctime
			ms.write( &iTmp16, 2 );		// Name size
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

Bu::MyriadStream Bu::MyriadFs::open( const Bu::String &sPath, int iMode )
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

		// The file wasn't found, but the path leading up to it was.
		// first, figure out the final path element...
		Bu::String::const_iterator iStart = sPath.begin();
		if( *iStart == '/' )
			iStart++;
		sio << "Scanning for filename:" << sio.nl;
		for( Bu::String::const_iterator iEnd = iStart.find('/')+1; iEnd; iStart = iEnd ) { }
		Bu::String sName( iStart, sPath.end() );
		sio << "End filename: " << sName << sio.nl;
		sio << "Parent inode: " << iParent << sio.nl;
		iNode = create( iParent, sName, 0664|typeRegFile );
		sio << "New iNode: " << iNode << sio.nl;
		return openByInode( iNode );
	}

	return mStore.openStream( 2 );
}

int32_t Bu::MyriadFs::lookupInode( const Bu::String &sPath, int32_t &iParent )
{
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

	sio << "Direcotry component: " << sTok << sio.nl;

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
					return lookupInode( iEnd+1, (*i).iNode );
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

	throw Bu::MyriadFsException( 1, "Path not found");
}

Bu::MyriadFs::Dir Bu::MyriadFs::readDir( int32_t iNode )
{
	Bu::MyriadStream ms = openByInode( iNode );
	int32_t iNumChildren;
	ms.read( &iNumChildren, 4 );
	
	Bu::MyriadStream is = mStore.openStream( 2 );
	Dir lDir;
	sio << "Reading dir, " << iNumChildren << " entries:" << sio.nl;
	for( int32_t j = 0; j < iNumChildren; j++ )
	{
		int32_t iChildNode;
		ms.read( &iChildNode, 4 );
		Stat s;
		stat( iChildNode, s, is );
		lDir.append( s );

		sio << "    " << s.sName << sio.nl;
	}

	return lDir;
}

Bu::MyriadStream Bu::MyriadFs::openByInode( int32_t iNode )
{
	int32_t iIndex = hNodeIndex.get( iNode );
	Bu::MyriadStream ms = mStore.openStream( 2 );
	ms.setPos( mStore.getBlockSize()*iIndex );
	RawStat rs;
	ms.read( &rs, sizeof(RawStat) );
	switch( (rs.uPerms&typeMask) )
	{
		case typeDir:
		case typeSymLink:
		case typeRegFile:
			return mStore.openStream( rs.iStreamIndex );

		default:
			throw Bu::MyriadFsException(
				"inode incorrect type for low-level openByInode.");
	}
}

int32_t Bu::MyriadFs::create( int32_t iParent, const Bu::String &sName,
		uint16_t uPerms )
{
	Bu::MyriadStream ms = openByInode( iParent );
	int32_t iNumChildren;
	ms.read( &iNumChildren, 4 );
	iNumChildren++;
	ms.setPos( 0 );
	ms.write( &iNumChildren, 4 );
	ms.setPos( iNumChildren*4 ); // Actually 4+(iNumChildren-1)*4 :-P
	int32_t iNode = allocInode( sName, iParent, uPerms );
	ms.write( &iNode, 4 );
	return iNode;
}

int32_t Bu::MyriadFs::allocInode( const Bu::String &sName, int32_t iParent,
		uint16_t uPerms )
{
	int32_t iNode = 0;
	for(; iNode < 0xfffffff; iNode++ )
	{
		if( !hNodeIndex.has( iNode ) )
		{
			Bu::MyriadStream is = mStore.openStream( 2 );
			is.setSize( (hNodeIndex.getSize()+1)*mStore.getBlockSize() );
			is.setPos( hNodeIndex.getSize()*mStore.getBlockSize() );

			hNodeIndex.insert( iNode, hNodeIndex.getSize() );
			RawStat rs;
			rs.iUser = iUser;
			rs.iGroup = iGroup;
			rs.uPerms = uPerms;
			rs.iLinks = 1;
			switch( (uPerms&typeMask) )
			{
				case typeDir:
				case typeRegFile:
				case typeSymLink:
					rs.iStreamIndex = mStore.createStream();
					break;

				default:
					rs.iStreamIndex = 0;
					break;
			}
			rs.iParentNode = iParent;
			rs.iATime = time(NULL);
			rs.iMTime = time(NULL);
			rs.iCTime = time(NULL);
			rs.iNameSize = sName.getSize();
			is.write( &rs, sizeof(RawStat) );
			is.write( sName.getStr(), sName.getSize() );

			return iNode;
		}
	}

	throw Bu::MyriadFsException(
		"No inode could be allocated.  You've run out!");
}

void Bu::MyriadFs::stat( int32_t iNode, Stat &rBuf, MyriadStream &rIs )
{
	rIs.setPos( hNodeIndex.get( iNode )*mStore.getBlockSize() );
	RawStat rs;
	rIs.read( &rs, sizeof(RawStat) );
	rBuf.sName.setSize( rs.iNameSize );
	rIs.read( rBuf.sName.getStr(), rs.iNameSize );
	rBuf.iNode = iNode;
	rBuf.iUser = rs.iUser;
	rBuf.iGroup = rs.iGroup;
	rBuf.uPerms = rs.uPerms;
	rBuf.iLinks = rs.iLinks;
	rBuf.iATime = rs.iATime;
	rBuf.iMTime = rs.iMTime;
	rBuf.iCTime = rs.iCTime;
	switch( (rBuf.uPerms&typeMask) )
	{
		case typeRegFile:
		case typeSymLink:
			rBuf.iSize = mStore.getStreamSize( rs.iStreamIndex );
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

