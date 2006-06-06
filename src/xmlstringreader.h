#ifndef XMLSTRINGREADER
#define XMLSTRINGREADER

#include <stdio.h>
#include "xmlreader.h"
#include "flexbuf.h"

/**
 * Takes care of reading in xml formatted data from a file.  This could/should
 * be made more arbitrary in the future so that we can read the data from any
 * source.  This is actually made quite simple already since all data read in
 * is handled by one single helper function and then palced into a FlexBuf for
 * easy access by the other functions.  The FlexBuf also allows for block
 * reading from disk, which improves speed by a noticable amount.
 * <br>
 * There are also some extra features implemented that allow you to break the
 * standard XML reader specs and eliminate leading and trailing whitespace in
 * all read content.  This is useful in situations where you allow additional
 * whitespace in the files to make them easily human readable.  The resturned
 * content will be NULL in sitautions where all content between nodes was
 * stripped.
 *@author Mike Buland
 */
class XmlStringReader : public XmlReader
{
public:
	/**
	 * Create a new string reader around an already created and formatted
	 * null-terminated string.
	 *@param sString A pointer to the string data that will be used.  This data
	 * is not changed during processing.
	 *@param bStrip Strip out leading and trailing whitespace.
	 */
	XmlStringReader( const char *sString, bool bStrip=false );

	/**
	 * Destroy this string reader.
	 */
	~XmlStringReader();

private:
	char getChar( int nIndex = 0 );
	void usedChar( int nAmnt = 1 );
	const char *sString;	/**< Internal pointer to the input string. */
	int nIndex;		/**< Our index into the string */
	int nLength;	/**< The computed length of the string */
};

#endif
