/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MYRIAD_H
#define BU_MYRIAD_H

#include <stdint.h>
#include "bu/bitstring.h"
#include "bu/exceptionbase.h"
#include "bu/array.h"
#include "bu/hash.h"

namespace Bu
{
	class Stream;
	class MyriadStream;

	subExceptionDecl( MyriadException )

	/**
	 * Numerically Indexed Data Streams.  This is a working name so I can
	 * actually get some code written instead of agonizing over the name.
	 *
	 * This is a system for creating streams that contain other streams in
	 * a flexible block-allocated system.
	 *
	 * Header format is as follows:
	 *
	 * MMMMvBssssSSSS*
	 * M = Magic number (FFC399BD)
	 * v = version number
	 * B = Bits per int
	 * s = Blocksize in bytes
	 * S = Number of Streams
	 * 
	 * The * represents the Stream headers, one per stream, as follows:
	 * IIIIssss$
	 * I = Id number of the stream
	 * s = size of stream in bytes
	 *
	 * The $ represents the Block headers, one per used block, as  follows:
	 * IIII
	 * I = Index of the block
	 *
	 * The stream/block data is interleaved in the header, so all blocks stored
	 * with one stream are together.  The block headers are in order, and the
	 * data in them is required to be "solid" you cannot fill partial blocks
	 * mid-way through a stream.
	 *
	 * The initial block starts with the nids header, and is both the zero block
	 * and the zero stream.  For now, the minimum block size is the size needed
	 * to store the base header, the zero stream header, and the first two
	 * blocks of the zero stream, so 30 bytes.  Since it's reccomended to use
	 * a size that will fit evenly into filesystem blocks, then a size of 32 is
	 * probably the smallest reccomended size because all powers of two equal
	 * to or greater than 32 are evenly divisible by 32.
	 *
	 * I have had a thought that if the block size were smaller than 42 bytes
	 * the header would consume the first N blocks where N * block size is
	 * enough space to house the initial header, the first stream header, and
	 * the first N block headers.  This, of course, causes you to hit an
	 * infinite header if the block size is small enough.
	 */
	class Myriad
	{
		friend class MyriadStream;
	public:
		Myriad( Bu::Stream &sStore );
		virtual ~Myriad();

		/**
		 * Initialize this object based on the data already in the assosiated
		 * stream.  This will be called automatically for you if you forget,
		 * but if you want to pre-initialize for some reason, just call this
		 * once before you actually start doing anything with your Myriad.
		 */
		void initialize();

		/**
		 * Create a new Myriad system in the assosiated stream.  This should be
		 * used carefully, it will destroy all data already within the stream.
		 * More options will probably be added soon.
		 */
		void initialize( int iBlockSize, int iPreAllocate=1 );

		/**
		 * Create a new stream within the Myriad system.  The ID of the new stream
		 * is returned.
		 */
		int createStream( int iPreAllocate=1 );

		/**
		 * Delete a stream that's already within the Myriad.
		 */
		void deleteStream( int iId );

		/**
		 * Return a new Stream object assosiated with the given stream ID.
		 */
		MyriadStream openStream( int iId );

		int getBlockSize();
		int getNumBlocks();
		int getNumUsedBlocks();
		int getBlockOverhead();

		/**
		 * Syncronize the header data, etc. with the storage stream.  It's not
		 * a bad idea to call this periodically.
		 */
		void sync();

	private:
		enum
		{
			blockUnused	=	0xFFFFFFFFUL
		};
		
		typedef Bu::Array<int> BlockArray;
		class Stream
		{
		public:
			int iId;
			int iSize;
			BlockArray aBlocks;
		};
		typedef Bu::Array<Stream *> StreamArray;

		class Block
		{
		public:
			char *pData;
			bool bChanged;
			int iBlockIndex;
		};

		void updateHeader();
		int findEmptyBlock();

		/**
		 *@todo Change this to use a binary search, it's nicer.
		 */
		Stream *findStream( int iId );

		Block *getBlock( int iBlock );
		void releaseBlock( Block *pBlock );
		void syncBlock( Block *pBlock );

		void setStreamSize( Stream *pStream, long iSize );

	private:
		Bu::Stream &sStore;
		int iBlockSize;
		int iBlocks;
		int iUsed;
		Bu::BitString bsBlockUsed;
		StreamArray aStreams;
		typedef Bu::Hash<int, Block *> BlockHash;
		BlockHash hActiveBlocks;
		bool bHeaderChanged;
	};
};

#endif
