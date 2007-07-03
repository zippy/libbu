#ifndef XML_STRING_WRITER
#define XML_STRING_WRITER

#include "xmlnode.h"
#include "xmlwriter.h"

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
class XmlStringWriter : public XmlWriter
{
public:
	/**
	 * Construct a string writer using an internal string buffer.
	 *@param sIndent Optional indent to add to each line.
	 */
	XmlStringWriter( const char *sIndent=NULL );

	/**
	 * Destroy the string writer and the internal string.
	 */
	virtual ~XmlStringWriter();

	/**
	 * Get the string that was built.  This is only valid after the document has
	 * been completed, so check isCompleted or be sure your addNode and
	 * closeNode calls match up.
	 *@returns A reference to the internal string object.
	 */
	std::string &getString();

private:
	void writeString( const char *sString );
	std::string sXml;	/**< The string object we "write" to. */
};

#endif
