/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef MYRIAD_FS_H
#define MYRIAD_FS_H

#include "bu/myriad.h"

namespace Bu
{
	class Stream;

	subExceptionDecl( MyriadFsException );

	/**
	 * A POSIX compliant, node based filesystem built on top of Myriad.
	 *
	 * A header is placed into stream 1.
	 * Header format:
	 *   int32_t iMagicHeader  (A7188B39)
	 *   int8_t iVersion (1)
	 *   int32_t iNumNodes
	 *   NodeLookup[iNumNodes] nNode
	 *
	 * Node lookup:
	 *   int32_t iInode
	 *   int32_t iPosition
	 *
	 * The node headers or inode structures have a base size of 46 bytes.
	 * Everything else in the block is used for the name.  I.e. if you have
	 * a blocksize of 512 bytes then you wind up with a max name size of
	 * 512-46=466 characters, or a blocksize of 256 gives you 210 chraacters
	 * as a max.  The node headers are all stored in stream 2.
	 * Basic node header format:
	 *   int32_t iUser
	 *   int32_t iGroup
	 *   int16_t iPerms
	 *   int16_t iLinks
	 *   int32_t iStreamIndex
	 *   int32_t iParentNode
	 *   int64_t iATime
	 *   int64_t iMTime
	 *   int64_t iCTime
	 *   int16_t iNameSize
	 *   char[iNameSize] sName
	 *
	 * Some types get special formats for their assosiated data stream, or
	 * other special considerations, here's a list:
	 *
	 * - typeFifo: No stream, iStreamIndex unused (probably)
	 * - typeChrDev: No stream, iStreamIndex is device hi/lo
	 * - typeDir: The stream contains a directory contents listing, described
	 *            below
	 * - typeBlkDev: No stream, iStreamIndex is device hi/lo
	 * - typeRegFile: The stream is the file data
	 * - typeSymLink: The stream is the destination of the symlink
	 * - typeSocket: No steram, iStreamIndex unused (probably)
	 *
	 * Directory streams have this simple listing format.  They contain a list
	 * of all child elements, with no particular order at the moment.  The . and
	 * .. entries are not listed, they are implicit:
	 *   int32_t iNumNodes
	 *   NodeTable[iNumNodes] nChildren
	 *
	 * NodeTable:
	 *   int32_t iInode
	 */
	class MyriadFs
	{
	public:
		MyriadFs( Bu::Stream &rStore, int iBlockSize=512 );
		virtual ~MyriadFs();

		enum
		{
			permOthX	= 0000001,
			permOthW	= 0000002,
			permOthR	= 0000004,
			permGrpX	= 0000010,
			permGrpW	= 0000020,
			permGrpR	= 0000040,
			permUsrX	= 0000100,
			permUsrW	= 0000200,
			permUsrR	= 0000400,
			permSticky	= 0001000,
			permSetGid	= 0002000,
			permSetUid	= 0004000,
			typeFifo	= 0010000,
			typeChrDev	= 0020000,
			typeDir		= 0040000,
			typeBlkDev	= 0060000,
			typeRegFile	= 0100000,
			typeSymLink	= 0120000,
			typeSocket	= 0140000,
			typeMask	= 0170000
		};

		enum
		{
			Read		= 0x01,	///< Open file for reading
			Write		= 0x02, ///< Open file for writing
			Create		= 0x04, ///< Create file if it doesn't exist
			Truncate	= 0x08, ///< Truncate file if it does exist
			Append		= 0x10, ///< Always append on every write
			NonBlock	= 0x20, ///< Open file in non-blocking mode
			Exclusive	= 0x44, ///< Create file, if it exists then fail

			// Helpful mixes
			ReadWrite	= 0x03, ///< Open for reading and writing
			WriteNew	= 0x0E	///< Create a file (or truncate) for writing.
								/// Same as Write|Create|Truncate
		};
		
		class Stat
		{
		public:
			int32_t iNode;
			int32_t iUser;
			int32_t iGroup;
			uint16_t uPerms;
			int16_t iLinks;
			int64_t iATime;
			int64_t iMTime;
			int64_t iCTime;
			int32_t iSize;
			Bu::String sName;
		};
		typedef Bu::List<Stat> Dir;

		void stat( const Bu::String &sPath, Stat &rBuf );
		MyriadStream open( const Bu::String &sPath, int iMode );
//		void create( const Bu::String &sPath, uint16_t iPerms );


	private:
		class RawStat
		{
		public:
			int32_t iUser;
			int32_t iGroup;
			uint16_t uPerms;
			int16_t iLinks;
			int32_t iStreamIndex;
			int32_t iParentNode;
			int64_t iATime;
			int64_t iMTime;
			int64_t iCTime;
			int16_t iNameSize;
		};
		typedef Bu::Hash<int32_t, int32_t> NodeIndex;

	private:
		int32_t lookupInode( const Bu::String &sPath, int32_t &iParent );
		int32_t lookupInode( Bu::String::const_iterator iStart,
				int32_t iNode, int32_t &iParent );
		Dir readDir( int32_t iNode );
		MyriadStream openByInode( int32_t iNode );
		int32_t create( int32_t iParent, const Bu::String &sName,
				uint16_t uPerms );
		int32_t allocInode( const Bu::String &sName, int32_t iParent,
				uint16_t uPerms );
		void stat( int32_t iNode, Stat &rBuf, MyriadStream &rIs );
		void writeHeader();

	private:
		Bu::Stream &rStore;
		Bu::Myriad mStore;
		NodeIndex hNodeIndex;
		int32_t iUser;
		int32_t iGroup;
	};
};

#endif
