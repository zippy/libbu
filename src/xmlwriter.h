#ifndef XMLWRITER
#define XMLWRITER

#include "xmlnode.h"
#include "xmldocument.h"

/**
 * Implements xml writing in the XML standard format.  Also allows you to
 * break that format and auto-indent your exported xml data for ease of
 * reading.  The auto-indenting will only be applied to sections that
 * have no content of their own already.  This means that except for
 * whitespace all of your data will be preserved perfectly.
 * You can create an XmlWriter object around a file, or access the static
 * write function directly and just hand it a filename and a root XmlNode.
 * When using an XmlWriter object the interface is identicle to that of
 * the XmlDocument class, so reference that class for API info.  However
 * when the initial (or root) node is closed, and the document is finished
 * the file will be created and written to automatically.  The user can
 * check to see if this is actually true by calling the isFinished
 * function in the XmlDocument class.
 *@author Mike Buland
 */
class XmlWriter : public XmlDocument
{
public:
	/**
	 * Construct a standard XmlWriter.
	 *@param sIndent Set this to something other than NULL to include it as an
	 * indent before each node in the output that doesn't already have content.
	 * If you are using the whitespace stripping option in the XmlReader and set
	 * this to a tab or some spaces it will never effect the content of your
	 * file.
	 */
	XmlWriter( const char *sIndent=NULL, XmlNode *pRoot=NULL );

	/**
	 * Destroy the writer.
	 */
	virtual ~XmlWriter();

	/**
	 * This override of the parent class closeNode function calls the parent
	 * class, but also triggers a write operation when the final node is closed.
	 * This means that by checking the isCompleted() function the user may also
	 * check to see if their file has been written or not.
	 */
	void closeNode();

	void write();

private:
	std::string sIndent;	/**< The indent string */

	std::string escape( std::string sIn );
	
	/**
	 * Write the file.
	 *@param pNode The root node
	 *@param sIndent The indent text.
	 */
	void write( XmlNode *pNode, const char *sIndent=NULL );

	/**
	 * Write a node in the file, including children.
	 *@param pNode The node to write.
	 *@param nIndent The indent level (the number of times to include sIndent)
	 *@param sIndent The indent text.
	 */
	void writeNode( XmlNode *pNode, int nIndent, const char *sIndent );

	/**
	 * Write the properties of a node.
	 *@param pNode The node who's properties to write.
	 *@param nIndent The indent level of the containing node
	 *@param sIndent The indent text.
	 */
	void writeNodeProps( XmlNode *pNode, int nIndent, const char *sIndent );

	/**
	 * Called to write the actual indent.
	 *@param nIndent The indent level.
	 *@param sIndent The indent text.
	 */
	void writeIndent( int nIndent, const char *sIndent );

	/**
	 * This is the function that must be overridden in order to use this class.
	 * It must write the null-terminated string sString, minus the mull,
	 * verbatum to it's output device.  Adding extra characters for any reason
	 * will break the XML formatting.
	 *@param sString The string data to write to the output.
	 */
	virtual void writeString( const char *sString ) = 0;
};

#endif
