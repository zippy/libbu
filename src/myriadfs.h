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
	 * Think about putting this all in one stream, on block boundaries.
	 *
	 * A header is placed into stream 1.
	 * Header format:
	 *   int32_t iMagicHeader  (A7188B39)
	 *   int8_t iVersion (1)
	 *   int32_t iNodeSize
	 *   int32_t iNumNodes
	 *   NodeLookup[iNumNodes] nNode
	 *
	 * Node lookup:
	 *   int32_t iInode
	 *   int32_t iPosition
	 *
	 * The node headers or inode structures have a base size of 22 bytes.
	 * Everything else in the block is used for the name.  I.e. if you have
	 * a blocksize of 512 bytes then you wind up with a max name size of
	 * 512-22=490 characters, or a blocksize of 256 gives you 234 chraacters
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
		}

	private:
		Bu::Stream &rStore;
		Bu::Myriad mStore;
	};
};

#endif
