#ifndef XMLREADER
#define XMLREADER

#include <stdio.h>
#include "xmldocument.h"
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
class XmlReader : public XmlDocument
{
public:
	/**
	 * Create a standard XmlReader.  The optional parameter bStrip allows you to
	 * create a reader that will strip out all leading and trailing whitespace
	 * in content, a-la html.
	 *@param bStrip Strip out leading and trailing whitespace?
	 */
	XmlReader( bool bStrip=false );

	/**
	 * Destroy this XmlReader.
	 */
	~XmlReader();

	/**
	 * Build a document based on some kind of input.  This is called
	 * automatically by the constructor.
	 */
	bool buildDoc();

private:
	/**
	 * This is called by the low level automoton in order to get the next
	 * character.  This function should return a character at the current
	 * position plus nIndex, but does not increment the current character.
	 *@param nIndex The index of the character from the current stream position.
	 *@returns A single character at the requested position, or 0 for end of
	 * stream.
	 */
	virtual char getChar( int nIndex = 0 ) = 0;

	/**
	 * Called to increment the current stream position by a single character.
	 */
	virtual void usedChar() = 0;

	/**
	 * Automoton function: is whitespace.
	 *@param chr A character
	 *@returns True if chr is whitespace, false otherwise.
	 */
	bool isws( char chr );

	/**
	 * Automoton function: ws.  Skips sections of whitespace.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool ws();

	/**
	 * Automoton function: node.  Processes an XmlNode
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool node();

	/**
	 * Automoton function: startNode.  Processes the begining of a node.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool startNode();

	/**
	 * Automoton function: name.  Processes the name of a node.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool name();

	char getEscape();

	/**
	 * Automoton function: paramlist.  Processes a list of node params.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool paramlist();

	/**
	 * Automoton function: param.  Processes a single parameter.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool param();

	/**
	 * Automoton function: content.  Processes node content.
	 *@returns True if everything was ok, False for end of stream.
	 */
	bool content();
	
	FlexBuf fbContent;		/**< buffer for the current node's content. */
	FlexBuf fbParamName;	/**< buffer for the current param's name. */
	FlexBuf fbParamValue;	/**< buffer for the current param's value. */
	bool bStrip;	/**< Are we stripping whitespace? */
};

#endif
