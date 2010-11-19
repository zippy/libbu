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
	 *   int16_t iMeta
	 *   int16_t iPerms
	 *   int32_t iStreamIndex
	 *   int32_t iParentNode
	 *   int16_t iNameSize
	 *   char[iNameSize] sName
	 */
	class MyriadFs
	{
	public:
		MyriadFs( Bu::Stream &rStore, int iBlockSize=512 );
		virtual ~MyriadFs();

	private:
		Bu::Stream &rStore;
		Bu::Myriad mStore;
	};
};

#endif
