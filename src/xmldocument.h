#ifndef XMLDOCUMENT
#define XMLDOCUMENT

#include "xmlnode.h"

/**
 * Keeps track of an easily managed set of XmlNode information.  Allows simple
 * operations for logical writing to and reading from XML structures.  Using
 * already formed structures is simply done through the XmlNode structures,
 * and the getRoot function here.  Creation is performed through a simple set
 * of operations that creates the data in a stream type format.
 *@author Mike Buland
 */
class XmlDocument
{
public:
	/**
	 * Construct either a blank XmlDocuemnt or construct a document around an
	 * existing XmlNode.  Be careful, once an XmlNode is passed into a document
	 * the document takes over ownership and will delete it when the XmlDocument
	 * is deleted.
	 *@param pRoot The XmlNode to use as the root of this document, or NULL if
	 * you want to start a new document.
	 */
	XmlDocument( XmlNode *pRoot=NULL );

	/**
	 * Destroy all contained nodes.
	 */
	virtual ~XmlDocument();

	/**
	 * Add a new node to the document.  The new node is appended to the end of
	 * the current context, i.e. XmlNode, and the new node, provided it isn't
	 * close as part of this operation, will become the current context.
	 *@param sName The name of the new node to add.
	 *@param sContent A content string to be placed inside of the new node.
	 *@param bClose Set this to true to close the node immediately after adding
	 * the node and setting the content and name.  If this is set to true the
	 * node is appended, but the context node doesn't change.
	 */
	void addNode( const char *sName=NULL, const char *sContent=NULL, bool bClose=false );

	/**
	 * Set the name of the current node context.
	 *@param sName The new name of the node.
	 */
	void setName( const char *sName );

	/**
	 * Close the current node context.  This will move the current context to
	 * the parent node of the former current node.  If the current node was the
	 * root then the "completed" flag is set and no more operations are allowed.
	 */
	void closeNode();

	/**
	 * Change the content of the current node at the current position between
	 * nodes.
	 *@param sContent The new content of the current node.
	 */
	void setContent( const char *sContent );

	/**
	 * Add a named property to the current context node.
	 *@param sName The name of the property to add.
	 *@param sValue The string value of the property.
	 */
	void addProperty( const char *sName, const char *sValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const unsigned char nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const char nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const unsigned short nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const short nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const unsigned long nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const long nValue );
	
	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param nValue The numerical value to add.
	 */
	void addProperty( const char *sName, const int nValue );

	/**
	 * Add a named property to the current context node, converting the
	 * numerical parameter to text using standrd printf style conversion.
	 *@param sName The name of the property to add.
	 *@param dValue The numerical value to add.
	 */
	void addProperty( const char *sName, const double dValue );

	/**
	 * The XmlDocuemnt is considered completed if the root node has been closed.
	 * Once an XmlDocument has been completed, you can no longer perform
	 * operations on it.
	 *@return True if completed, false if still in progress.
	 */
	bool isCompleted();

	/**
	 * Get a pointer to the root object of this XmlDocument.
	 *@returns A pointer to an internally owned XmlNode.  Do not delete this
	 * XmlNode.
	 */
	XmlNode *getRoot();

	/**
	 * Get a pointer to the root object of this XmlDocument, and remove the
	 * ownership from this object.
	 *@returns A pointer to an internally owned XmlNode.  Do not delete this
	 * XmlNode.
	 */
	XmlNode *detatchRoot();

	/**
	 * Get the current context node, which could be the same as the root node.
	 *@returns A pointer to an internally owned XmlNode.  Do not delete this
	 * XmlNode.
	 */
	XmlNode *getCurrent();

private:
	XmlNode *pRoot;		/**< The root node. */
	XmlNode *pCurrent;	/**< The current node. */
	bool bCompleted;	/**< Is it completed? */
};

#endif
