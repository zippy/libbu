/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_NIDS_H
#define BU_NIDS_H

#include <stdint.h>
#include "bu/bitstring.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	class Stream;
	class NidsStream;

	subExceptionDecl( NidsException )

	/**
	 * Numerically Indexed Data Streams.  This is a working name so I can
	 * actually get some code written instead of agonizing over the name.
	 *
	 * This is a system for creating streams that contain other streams in
	 * a flexible block-allocated system.
	 */
	class Nids
	{
		friend class NidsStream;
	public:
		Nids( Bu::Stream &sStore );
		virtual ~Nids();

		/**
		 * Initialize this object based on the data already in the assosiated
		 * stream.  This will be called automatically for you if you forget,
		 * but if you want to pre-initialize for some reason, just call this
		 * once before you actually start doing anything with your Nids.
		 */
		void initialize();

		/**
		 * Create a new Nids system in the assosiated stream.  This should be
		 * used carefully, it will destroy all data already within the stream.
		 * More options will probably be added soon.
		 */
		void initialize( int iBlockSize, int iPreAllocate=1 );

		/**
		 * Create a new stream within the Nids system.  The ID of the new stream
		 * is returned.
		 */
		int createStream( int iPreAllocate=1 );

		/**
		 * Delete a stream that's already within the Nids.
		 */
		void deleteStream( int iID );

		/**
		 * Return a new Stream object assosiated with the given stream ID.
		 */
		NidsStream openStream( int iID );

		int getBlockSize();
		int getNumBlocks();
		int getNumUsedBlocks();
		int getBlockStart();
		int getBlockOverhead();

	private:
		typedef struct Block
		{
			uint32_t uFirstBlock;
			uint32_t uNextBlock;
			// uint32_t uPrevBlock;
			uint32_t uBytesUsed;
			// uint32_t uReserved;
			unsigned char pData[0];
		} Block;

		enum
		{
			blockUnused	=	0xFFFFFFFFUL
		};

		void initBlock( uint32_t uPos, uint32_t uFirstBlock,
			/*uint32_t uPrevBlock,*/ bool bNew=false );
		uint32_t createBlock( uint32_t uFirstBlock, /*uint32_t uPrevBlock,*/
			int iPreAllocate=1 );
		void getBlock( uint32_t uIndex, struct Nids::Block *pBlock );
		void setBlock( uint32_t uIndex, struct Nids::Block *pBlock );
		void updateStreamSize( uint32_t uIndex, uint32_t uSize );
		uint32_t getNextBlock( uint32_t uIndex, struct Nids::Block *pBlock,
			bool bCreate=true);

		void updateHeader();

		// Block allocation routines
		Block *newBlock();
		void deleteBlock( Block *pBlock );

	private:
		Bu::Stream &sStore;
		int iBlockSize;
		int iBlocks;
		int iBlockStart;
		int iUsed;
		Bu::BitString bsBlockUsed;
	};
};

#endif
