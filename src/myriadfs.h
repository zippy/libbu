/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef MYRIAD_FS_H
#define MYRIAD_FS_H

#include <sys/types.h>

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
	 * The node headers or inode structures have a base size of 44 bytes.
	 * The name is stored in the directory format.
	 * Basic node header format:
	 *   int32_t iNode
	 *   int32_t iUser
	 *   int32_t iGroup
	 *   uint16_t uPerms
	 *   int16_t iLinks
	 *   uint32_t uStreamIndex
	 *   int64_t iATime
	 *   int64_t iMTime
	 *   int64_t iCTime
	 *
	 * Some types get special formats for their assosiated data stream, or
	 * other special considerations, here's a list:
	 *
	 * - typeFifo: No stream, uStreamIndex unused (probably)
	 * - typeChrDev: No stream, uStreamIndex is device hi/lo
	 * - typeDir: The stream contains a directory contents listing, described
	 *            below
	 * - typeBlkDev: No stream, uStreamIndex is device hi/lo
	 * - typeRegFile: The stream is the file data
	 * - typeSymLink: The stream is the destination of the symlink
	 * - typeSocket: No steram, uStreamIndex unused (probably)
	 *
	 * Directory streams have this simple listing format.  They contain a list
	 * of all child elements, with no particular order at the moment.  The . and
	 * .. entries are not listed, they are implicit:
	 *   int32_t iNumNodes
	 *   NodeTable[iNumNodes] nChildren
	 *
	 * NodeTable:
	 *   int32_t iInode
	 *   uint8_t uNameSize
	 *   char[uNameSize] sName
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
			permMask	= 0007777,
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
			uint32_t uDev;
			Bu::String sName;
		};
		typedef Bu::List<Stat> Dir;

		void stat( const Bu::String &sPath, Stat &rBuf );
		MyriadStream open( const Bu::String &sPath, int iMode,
			uint16_t uPerms=0664 );
		void create( const Bu::String &sPath, uint16_t iPerms );
		void create( const Bu::String &sPath, uint16_t iPerms,
				uint16_t iDevHi, uint16_t iDevLo );
		void create( const Bu::String &sPath, uint16_t iPerms,
				uint32_t uSpecial );
		void mkDir( const Bu::String &sPath, uint16_t iPerms );
		void mkSymLink( const Bu::String &sPath, const Bu::String &sTarget );
		Bu::String readSymLink( const Bu::String &sPath );
		Dir readDir( const Bu::String &sPath );
		void setTimes( const Bu::String &sPath, int64_t iATime,
				int64_t iMTime );
		void unlink( const Bu::String &sPath );

		static dev_t devToSys( uint32_t uDev );
		static uint32_t sysToDev( dev_t uDev );

	private:
		class RawStat
		{
		public:
			int32_t iNode;
			int32_t iUser;
			int32_t iGroup;
			uint16_t uPerms;
			int16_t iLinks;
			uint32_t uStreamIndex;
			int64_t iATime;
			int64_t iMTime;
			int64_t iCTime;
		};
		typedef Bu::Hash<int32_t, int32_t> NodeIndex;

	private:
		int32_t lookupInode( const Bu::String &sPath, int32_t &iParent );
		int32_t lookupInode( Bu::String::const_iterator iStart,
				int32_t iNode, int32_t &iParent );
		void readInode( int32_t iNode, RawStat &rs, MyriadStream &rIs );
		void readInode( int32_t iNode, RawStat &rs );
		void writeInode( const RawStat &rs );
		void writeInode( const RawStat &rs, MyriadStream &rOs );
		Dir readDir( int32_t iNode );
		MyriadStream openByInode( int32_t iNode );
		int32_t create( int32_t iParent, const Bu::String &sName,
				uint16_t uPerms, uint32_t uSpecial );
		int32_t allocInode( uint16_t uPerms, uint32_t uSpecial );
		void stat( int32_t iNode, Stat &rBuf, MyriadStream &rIs );
		void writeHeader();
		void setTimes( int32_t iNode, int64_t iATime, int64_t iMTime );
		void destroyNode( int32_t iNode );

		Bu::String filePart( const Bu::String &sPath );

	private:
		Bu::Stream &rStore;
		Bu::Myriad mStore;
		NodeIndex hNodeIndex;
		int32_t iUser;
		int32_t iGroup;
	};
};

#endif
