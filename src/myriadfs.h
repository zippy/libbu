#ifndef MYRIAD_FS_H
#define MYRIAD_FS_H

namespace Bu
{
	/**
	 * A POSIX compliant, node based filesystem built on top of Myriad.
	 *
	 * Header format:
	 *   int32_t iMagicHeader
	 *   int8_t iVersion (1)
	 *   int32_t iNodeSize
	 *   int32_t iNumNodes
	 *   NodeLookup[iNumNodes] nNode
	 *
	 * Node lookup:
	 *   int32_t iInode
	 *   int32_t iPosition
	 *
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
		MyriadFs();
		virtual ~MyriadFs();
	};
};

#endif
