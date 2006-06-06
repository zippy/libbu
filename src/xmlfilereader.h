#ifndef XMLFILEREADER
#define XMLFILEREADER

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
class XmlFileReader : public XmlReader
{
public:
	/**
	 * Construct an XmlReader around an xml file on your file system.
	 *@param sFile The file to read.
	 *@param bStrip Set to true to strip out leading and trailing whitespace in
	 * node contents.
	 */
	XmlFileReader( const char *sFile, bool bStrip=false );

	/**
	 * Destroy the reader and cleanup.
	 */
	~XmlFileReader();

private:
	char getChar( int nIndex = 0 );
	void usedChar( int nAmnt = 1 );
	FILE *fh;			/**< The file handle. */
	FlexBuf fbDataIn;	/**< The input buffer. */
};

#endif
