#ifndef BU_NIDS_H
#define BU_NIDS_H

#include <stdint.h>
#include "bu/bitstring.h"

namespace Bu
{
	class Stream;
	class NidsStream;

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

	private:
		typedef struct Block
		{
			uint32_t iFirstBlock;
			uint32_t iNextBlock;
			uint32_t iPrevBlock;
			uint32_t iBytesUsed;
			uint32_t iReserved;
			unsigned char pData[0];
		} Block;

		enum
		{
			blockUnused	=	0xFFFFFFFFUL
		};

		void extendStream( int iID, int iBlockCount=1 );
		void getBlock( int iIndex, struct Nids::Block *pBlock );
		void setBlock( int iIndex, struct Nids::Block *pBlock );

	private:
		Bu::Stream &sStore;
		int iBlockSize;
		int iBlocks;
		int iBlockStart;
		Bu::BitString bsBlockUsed;
	};
};

#endif
