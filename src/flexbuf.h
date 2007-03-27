/**\flexbuf.h
 * Describes the FlexBuf class.
 *@author Mike Buland
 */

#ifndef FLEXBUF_H
#define FLEXBUF_H

/** Stores any amount of data, but starts small, growing as necesarry.
  * It is optimized to work with stream type situations, with data being
  * added to the end while it is being taken from the begning.
  *@todo Set this class up to auto-shrink back to a specified sized buffer each
  * time it has shrunk below that for enough operations.
  *@author Mike Buland
  */
class FlexBuf
{
public:
	/**
	 * Construct a blank FlexBuf containing about 1k of buffer space.
	 */
	FlexBuf();

	/**
	 * Clean up the FlexBuf, delete all buffers.
	 */
	virtual ~FlexBuf();

	/** Appends a whole string of data to the buffer.  The string
	  * must be null terminated.
	  *@param lpData The data to append to the buffer.
	  *@param nDSize The size of the data described by lpData.  If this
	  * value is -1 lpData is treated as a null-terminated string.
	  *@returns True if no problems occured, false otherwise.
	  */
	bool appendData( const char *lpData, int nDSize=-1 );

	/** Appends a single character to the end of the buffer.
	  *@param lData The character to append to the buffer.
	  *@returns True if no problems occured, false otherwise.
	  */
	bool appendData( const char lData );
	
	/**
	 * Append the short to the buffer.
	 *@param lData The short to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const short lData );

	/**
	 * Append the int to the buffer.
	 *@param lData The int to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const int lData );

	/**
	 * Append the long to the buffer.
	 *@param lData The long to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const long lData );

	/**
	 * Append the float to the buffer.
	 *@param lData The float to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const float lData );

	/**
	 * Append the double to the buffer.
	 *@param lData The double to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const double lData );

	/**
	 * Append the unsigned char to the buffer.
	 *@param lData The unsigned char to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const unsigned char lData );

	/**
	 * Append the unsigned short to the buffer.
	 *@param lData The unsigned short to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const unsigned short lData );

	/**
	 * Append the unsigned int to the buffer.
	 *@param lData The unsigned int to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const unsigned int lData );

	/**
	 * Append the unsigned long to the buffer.
	 *@param lData The unsigned long to add to the buffer queue.
	 *@returns True if everything is ok, false otherwise.
	 */
	bool appendData( const unsigned long lData );

	/** Removes all pending data from the buffer.
	  *@returns True if no problems occured, false otherwise.
	  */
	bool clearData();

	/** Gets a pointer to the internal buffer, at the begining of the current
	  * data stream.
	  *@returns A pointer to the internal data buffer.
	  */
	const char *getData();

	/** Gets the length of the current buffer (how much data is really in the
	  * buffer, not it's current capacity, for that check getCapacity)
	  *@returns The length of the current buffer.
	  */
	int getLength();

	/** Gets the current capacity of the FlexBuf.  If the size nears this value
	  * then the entire buffer is resized to accomidate more data.
	  *@returns The current capacity of the FlexBuf.
	  */
	int getCapacity();

	/**
	 * Removes nAmount bytes from the begning of the buffer.  Actually, if
	 * nAmount happens to be negative it will remove tha absolute value of
	 * nValue bytes from the end of the buffer, like the old delData command.
	 *@param nAmount The number of bytes used.
	 *@returns True if everything was successful, false if there was an error.
	 */
	bool usedData( int nAmount );

	/** Finds the first instance of the given character in the buffer and
	  * returns an index to it.
	  *@param cTarget The character you're looking for.
	  *@returns The index of the first instance of the given character, or
	  * -1 if it just wasn't found.
	  */
	int findChar( char cTarget );

	void ensureCapacity( int nAmount );

private:
	/** The raw storage location of the FlexBuf. */
	char *lpBuf;
	/** The real size of the FlexBuf.  */
	int nSize;
	/** Where the last char is. */
	int nLastChar;
	/** Where the first char is. */
	int nFirstChar;
	/** The amount of real data in the FlexBuf.  This is effectively nLastChar-nFirstChar. */
	int nFill;
};

#endif
