#ifndef BU_NIDS_H
#define BU_NIDS_H

namespace Bu
{
	class Stream;

	/**
	 * Numerically Indexed Data Streams.  This is a working name so I can
	 * actually get some code written instead of agonizing over the name.
	 *
	 * This is a system for creating streams that contain other streams in
	 * a flexible block-allocated system.
	 */
	class Nids
	{
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
		Bu::Stream &openStream( int iID );

	private:
		Bu::Stream &sStore;
	};
};

#endif
