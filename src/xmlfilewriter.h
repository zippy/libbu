#ifndef XML_FILE_WRITER
#define XML_FILE_WRITER

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
class XmlFileWriter : public XmlWriter
{
public:
	/**
	 * Construct a file writer around a given file.
	 *@param sFileName The file to create or overwrite and write XML into.
	 *@param sIndent The indent text to use, if any.
	 */
	XmlFileWriter( const char *sFileName, const char *sIndent=NULL, XmlNode *pRoot=NULL );

	/**
	 * Destroy the writer.
	 */
	virtual ~XmlFileWriter();

private:
	void writeString( const char *sString );
	std::string sFileName;	/**< The filename to write to. */
	FILE *fh;	/**< The file handle to the open file. */
};

#endif
