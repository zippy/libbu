#ifndef BU_BITSTRING_H
#define BU_BITSTRING_H

namespace Bu
{
	/**
	 * Manages an arbitrarily sized string of bits, and allows basic interaction
	 * with them.  This includes basic non-mathematical bitwise operations such
	 * as setting and testing bits, shifting the string, inversion and a few
	 * extras like randomization.  On linux systems this takes advantage of long
	 * longs giving you a maximum size of about 2tb per string.
	 *
	 * For more general and mathematical type interaction see BitStringInt.
	 * 
	 *@author Mike Buland
	 */
	class BitString
	{
	public:
		/**
		 * Constructs a blank and basic BitString.  This is actually useful since
		 * you can resize BitStrings at will, and even retain the data that was
		 * in them.
		 */
		BitString();

		/**
		 * Constructs a BitString object as a copy of another BitString.  This is
		 * a standard copy constructor and produces an exact duplicate of the
		 * original BitString object.
		 *@param xSrc Source BitString to copy data from.
		 */
		BitString( const BitString &xSrc );

		/**
		 * Constructs a BitString with length nBits and optionally fills it with
		 * random data.  The default setting, to not fill randomly, will produce
		 * a blank (all zeros) string of the specified size.
		 *@param nBits The length of the new BitString in bits.
		 *@param bFillRandomly Wether or not to randomize this BitString.
		 */
		BitString( long nBits, bool bFillRandomly=false );

		/**
		 * Virtual deconstructor for the BitString.  Takes care of cleanup for you.
		 * What more do you really want to know?
		 */
		virtual ~BitString();

		// basic interaction
		/**
		 * Sets a bit in the BitString.  In it's normal mode it will always turn
		 * the given bit on, to clear a bit set bBitState to false instead of
		 * true.  This operation runs in O(1).
		 *@param nBit The zero-based index of the bit to modify.
		 *@param bBitState Set to true to set the bit to 1, set to false to set
		 * the bit to 0.
		 */
		void setBit( long nBit, bool bBitState=true );

		/**
		 * Reverses the state of the given bit.  This will set the given bit to a
		 * 1 if it was 0, and to 0 if it was 1.  This operation runs in O(1), and
		 * it should be noted that using this is marginally faster than doing the
		 * test and flip yourself with getBit and setBit since it uses a bitwise
		 * not operation and doesn't actually test the bit itself.
		 *@param nBit The index of the bit to flip.
		 */
		void flipBit( long nBit );

		/**
		 * Gets the state of the given bit.  This follows the standard convention
		 * used so far, a returned value of true means the bit in question is 1,
		 * and a value of flase means the bit is 0.  All bits out of range of the
		 * BitString are treated as off, but are "accessable" in that this does not
		 * produce any kind of error message.  This is intentional.  This operation
		 * runs in O(1).
		 *@param nBit The index of the bit to test.
		 *@returns True for a 1, false for a 0.
		 */
		bool getBit( long nBit );

		/**
		 * Inverts the entire BitString, in effect this calls flipBit on every bit
		 * in the string but is faster since it can operate on whole bytes at a
		 * time instead of individual bits.  This operation runs in O(N).
		 */
		void invert();

		/**
		 * Returns the number of bits allocated in this BitString.  This operation
		 * runs in O(N) time since this value is cached and not computed.
		 *@returns The number of bits allocated in this BitString.
		 */
		long getBitLength();

		/**
		 * Sets the entire BitString to zeros, but it does it very quickly.  This
		 * operation runs in O(N).
		 */
		void clearString();

		/**
		 * Gets another BitString that is autonomous of the current one (contains
		 * a copy of the memory, not a pointer) and contains a subset of the data
		 * in the current BitString.  This is an inclusive operation, so grabbing
		 * bits 0-5 will give you 6 bits.  This is based on a very tricky
		 * bit-shifting algorithm and runs very quickly, in O(N) time.
		 * Passing in a value of zero for nUpper, or any value for nUpper that is
		 * less than nLower will set nUpper equal to the number of bits in the
		 * BitString.
		 *@param nLower The first bit in the current string, will be the first bit
		 * (0 index) in the new sub string.
		 *@param nUpper The last bit in the current string, will be the last bit in
		 * the new sub string.  nUpper is included in the sub string.
		 *@returns A new BitString object ready to be used.  Please note that
		 * managing this new object is up to whomever calls this function.
		 */
		class BitString getSubString( long nLower, long nUpper );

		/**
		 * Sets the number of bits in the BitString, allocating more memory if
		 * necesarry, or freeing extra if able.  The default operation of this
		 * function clears all data in the BitString while resizing it.  If you
		 * would like to keep as much of the data that you had in your BitString
		 * as possible, then set bClear to false, and any data that will fit into
		 * the new BitString length will be retained.  If increasing the number of
		 * bits, the new bits will come into existance cleared (set to 0).
		 *@param nLength The number of bits to set the BitString to.
		 *@param bClear When true, all data is eradicated and zeroed, when set to
		 * false an effort is made to retain the existing data.
		 *@returns true on success, false on failure.
		 */
		bool setBitLength( long nLength, bool bClear=true );

		/**
		 * Randomize the entire BitString, one bit at a time.  This is actually
		 * the function called by the constructor when the user selects initial
		 * randomization.  This function uses the system random() function, so
		 * srandom may be used to effect this process at will.
		 */
		void randomize();

		/**
		 * Operates exactly like <<.  All data in the BitString is shifted to
		 * the left by some number of bits, any data pushed off the edge of the
		 * BitString is lost, and all new data coming in will be zeroed.
		 * Using a negative value in the shiftLeft function will turn it into the
		 * shiftRight function.
		 *@param nAmt The number of bit positions to shift all data.
		 */
		void shiftLeft( long nAmt ); // just like <<

		/**
		 * Operates exactly like >>.  All data in the BitString is shifted to
		 * the right by some number of bits, any data pushed off the edge of the
		 * BitString is lost, and all new data coming in will be zeroed.
		 * Using a negative value in the shiftRight function will turn it into the
		 * shiftLeft function.
		 *@param nAmt The number of bit positions to shift all data.
		 */
		void shiftRight( long nAmt ); // just like >>

		/**
		 * Searches through the BitString and returns the index of the highest
		 * order bit position (the highest index) with an on bit (a bit set to 1).
		 * This is a handy helper function and rather faster than calling getBit()
		 * over and over again.
		 *@returns The index of the highest indexed on bit.
		 */
		long getHighestOrderBitPos();

		// Conversion
		/**
		 * Convert a block of data (no more than 32 bits) to a primitive long type.
		 * This is done in a little bit interesting way, so it may not always be
		 * the fastest way to access the data that you want, although it will
		 * always ensure that the long that is written makes numerical sense, as
		 * we write numbers, regaurdless of platform.
		 *@param nStart The first bit in the BitString to include in the long
		 *@param nSize THe number of bits to include, if this value is set over
		 * 32 it will be automatically truncated to, or however many bits there
		 * are in a long in your system.
		 *@returns A long converted from your raw BitString data.
		 */
		long toLong( long nStart = 0, long nSize = 32 );

		/**
		 * Converts the data into a human-readable SString object.  SString is
		 * used to make transport of the string and management very simple.  Since
		 * BitStrings will generally be longer than your average strip of ints a
		 * faculty is included and turned on by default that will insert spacers
		 * into the output text every 8 places.  For debugging work, this is
		 * definately reccomended.
		 *@param bAddSpacers Leave set to true in order to have the output broken
		 * into logical groupings of 8 bits per block.  Set to off to have a harder
		 * to read solid block of bits.
		 *@returns A SString object containing the produced string.
		 */
		//std::string toString( bool bAddSpacers = true );

		// Utility
		/**
		 * Converts the given number of bits into the smallest allocatable unit,
		 * which is bytes in C and on most systems nowadays.  This is the minimum
		 * number of bytes needed to contain the given number of bits, so there is
		 * generally some slop if they are not evenly divisible.
		 *@param nBits The number of bits you wish to use.
		 *@returns The number of bytes you will need to contain the given number
		 * of bits.
		 */
		//static long bitsToBytes( long nBits );

		/**
		 * Writes all data in the BitString, including a small header block
		 * describing the number of bits in the BitString to the file described
		 * by the given file descriptor.  The data writen is purely sequential and
		 * probably not too easy to read by other mechanisms, although the
		 * readFromFile function should always be able to do it.  This function
		 * does not open nor close the file pointed to by fh.
		 *@param fh The file descriptor of the file to write the data to.
		 *@returns true if the operation completed without error, false otherwise.
		 */
		//bool writeToFile( FILE *fh );

		/**
		 * Reads data formatted by writeToFile and clears out any data that may
		 * have been in the BitString.  This function preserves nothing in the
		 * original BitString that it may be replacing.  This function does not
		 * open nor close the file pointed to by fh.
		 *@param fh The file descriptor to try to read the data from.
		 *@returns true if the operation completed without error, false otherwise.
		 */
		//bool readFromFile( FILE *fh );

		//operators
		BitString &operator=( const BitString &xSrc );
		BitString operator~();
		BitString operator<<( const long nAmt );
		BitString operator>>( const long nAmt );

	private:
		void fixup();
		unsigned char *caData;
		long nBits;
		long nBytes;
		unsigned char cTopByteMask;
	};
};

#endif