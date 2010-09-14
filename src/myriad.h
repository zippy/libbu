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

	subExceptionDeclBegin( MyriadException )
		enum
		{
			emptyStream,
			invalidFormat,
			badVersion,
			invalidWordSize,
			noSuchStream,
			streamExists,
			invalidStreamId,
			protectedStream
		};
	subExceptionDeclEnd()

	/**
	 * Myriad block-allocated stream multiplexing system.  This is a system for
	 * creating streams that contain other streams in a flexible and lightweight
	 * manner.  Basically, you can create a file (or any other stream) that can
	 * store any number of flexible, growing streams.  The streams within the
	 * Myriad stream are automatically numbered, not named.  This works more
	 * or less like a filesystem, but without the extra layer for managing
	 * file and directory links.  This would actually be very easy to add
	 * on top of Myriad, but is not required.
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
		/**
		 * Create a Myriad object that uses the given stream to store data.
		 * This stream must be random access.  The block size and preallocate
		 * values passed in are values that will be used if the given stream
		 * is empty.  In that case the stream will be "formatted" for myriad
		 * with the specified block size.  If there is already a viable Myriad
		 * format present in the stream, then the blocksize and preallocate
		 * values will be ignored and the values from the stream will be used
		 * instead.  If the stream doesn't appear to be Myriad formatted an
		 * exception will be thrown.
		 */
		Myriad( Bu::Stream &sStore, int iBlockSize=512, int iPreallocate=8 );
		virtual ~Myriad();

		/**
		 * Destroy whatever data may be in the base stream and create a new
		 * Myriad system there with the given blocksize.  Use this with care,
		 * it will destroy anything that was already in the stream, and
		 * generally, should not ever have to be used.
		 */
		void initialize( int iBlockSize, int iPreAllocate=1 );

		/**
		 * Create a new stream within the Myriad system.  The ID of the new
		 * stream is returned.
		 */
		int createStream( int iPreAllocate=1 );

		/**
		 * Create a new stream within the Myriad system with a given id.  The
		 * id that you provide will be the new id of the stream unless it's
		 * already used, in which case an error is thrown.  This is primarilly
		 * useful when copying an old Myriad file into a new one.
		 */
		int createStreamWithId( int iId, int iPreAllocate=1 );

		/**
		 * Delete a stream that's already within the Myriad.
		 */
		void deleteStream( int iId );

		/**
		 * Return a new Stream object assosiated with the given stream ID.
		 */
		MyriadStream openStream( int iId );

		Bu::Array<int> getStreamIds();
		int getStreamSize( int iId );

		int getNumStreams();
		int getBlockSize();
		int getNumBlocks();
		int getNumUsedBlocks();

		/**
		 * Syncronize the header data, etc. with the storage stream.  It's not
		 * a bad idea to call this periodically.
		 */
		void sync();

		/**
		 * Read the first few bytes from the given stream and return true/false
		 * depending on weather or not it's a Myriad stream.  This will throw
		 * an exception if the stream is empty, or is not random access.
		 */
		static bool isMyriad( Bu::Stream &sStore );

	private:
		/**
		 * Initialize this object based on the data already in the assosiated
		 * stream.  This will be called automatically for you if you forget,
		 * but if you want to pre-initialize for some reason, just call this
		 * once before you actually start doing anything with your Myriad.
		 */
		void initialize();

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

		int streamAddBlock( Stream *pStream );
		void setStreamSize( Stream *pStream, long iSize );

		void headerChanged();

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
