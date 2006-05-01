#ifndef MD5_H
#define MD5_H

/**
 * Used to store an MD5 sum in a handy container.
 */
typedef struct
{
	/** The actual data-storage for an MD5 sum. */
	long data[4];
} md5sum;

/**
 * Class for easily calculating MD5 sums of just about any data.
 *@author Mike Buland
 */
class md5
{
public:
	/** Build an MD5 sum builder. */
	md5();

	/** Deconstruct */
	~md5();

	/**
	 * Create a sum of a standard c string, null terminated.  This is probably
	 * the easiest function to use.
	 *@param pSum The MD5 sum structure to fill up.
	 *@param sStr The null-terminated string to turn into an MD5 sum.
	 */
	void sumString( md5sum *pSum, const char *sStr );

	/**
	 * Create a sum of an array of arbitrary data.  This is the most handy for
	 * dealing with files and so on.
	 *@param pSum The MD5 sum structure to fill up.
	 *@param aData A pointer to the base of the data to sum.
	 *@param nLen The number of bytes to use in the sum.
	 */
	void sumData( md5sum *pSum, const char *aData, long nLen );

	/**
	 * Convert an md5sum to standard hex representation.  Make sure that sHex
	 * contains at least 17 characters of space.
	 *@param pSum The sum structure to convert to hex.
	 *@param sHex The string to store the hex value in.
	 */
	void sumToHex( md5sum *pSum, char *sHex );

private:
	/**
	 * Do the bulk of the work of the md5 algorithm.
	 *@param x I'm not sure.  I'll need to look it up.
	 *@param len The length of the data.
	 *@param output The sum structure to put the output in.
	 */
	void core_md5( long *x, long len, md5sum *output );

	/**
	 * Convert an array of charaters to an array of longs in a very crafty way.
	 * This also applies standard MD5 obfuscation to the resulting array, and
	 * makes it fit within MD5 size constraints.
	 *@param str The data to convert.
	 *@param len The length of the data.
	 *@param nNewLen A pointer to a variable that will hold the new length of
	 * the resulting array of longs.
	 *@returns The newly obfuscated and resized long array.
	 */
	long *c2l( const char *str, long len, long *nNewLen );

	/**
	 * Backend helper to convert an array of longs into a hex string.
	 *@param binarray The binary data to convert.
	 *@param str The string to store the hex string in.
	 */
	void l2hexstr( long *binarray, char *str );
	
};

#endif
