#ifndef XMLREADER
#define XMLREADER

#include <stdio.h>
#include "bu/xmldocument.h"
#include "bu/hash.h"
#include "bu/fstring.h"
#include "bu/stream.h"

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
	XmlReader( Bu::Stream &sIn, bool bStrip=false );

	/**
	 * Destroy this XmlReader.
	 */
	virtual ~XmlReader();

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
	virtual char getChar( int nIndex = 0 );

	/**
	 * Called to increment the current stream position by a single character.
	 */
	virtual void usedChar( int nAmnt = 1 );

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

	/**
	 * Automoton function: textDecl.  Processes the xml text decleration, if
	 * there is one.
	 */
	void textDecl();

	/**
	 * Automoton function: entity.  Processes an entity from the header.
	 */
	void entity();

	/**
	 * Adds an entity to the list, if it doesn't already exist.
	 *@param name The name of the entity
	 *@param value The value of the entity
	 */
	void addEntity( const Bu::FString &name, const Bu::FString &value );

	Bu::FString getEscape();

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
	
	Bu::FString sContent;		/**< buffer for the current node's content. */
	Bu::FString sParamName;		/**< buffer for the current param's name. */
	Bu::FString sParamValue;	/**< buffer for the current param's value. */
	Bu::Stream &sIn;
	bool bStrip;			/**< Are we stripping whitespace? */

	Bu::Hash<Bu::FString,Bu::FString> htEntity;		/**< Entity type definitions. */

	Bu::FString sBuf;
};

#endif