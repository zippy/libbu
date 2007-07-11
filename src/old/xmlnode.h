#ifndef XMLNODE
#define XMLNODE

#include <iostream>
#include "bu/list.h"
#include "bu/hash.h"
#include "bu/fstring.h"

/**
 * Maintains all data pertient to an XML node, including sub-nodes and content.
 * All child nodes can be accessed through index and through name via a hash
 * table.  This makes it very easy to gain simple and fast access to all of
 * your data.  For most applications, the memory footprint is also rather
 * small.  While XmlNode objects can be used directly to create XML structures
 * it is highly reccomended that all operations be performed through the
 * XmlDocument class.
 *@author Mike Buland
 */
class XmlNode
{
public:
	/**
	 * Construct a new XmlNode.
	 *@param sName The name of the node.
	 *@param pParent The parent node.
	 *@param sContent The initial content string.
	 */
	XmlNode(
		const Bu::FString &sName,
		XmlNode *pParent=NULL
		);
	
	/**
	 * Delete the node and cleanup all memory.
	 */
	virtual ~XmlNode();

	/**
	 * Change the name of the node.
	 *@param sName The new name of the node.
	 */
	//void setName( const char *sName );

	/**
	 * Construct a new node and add it as a child to this node, also return a
	 * pointer to the newly constructed node.
	 *@param sName The name of the new node.
	 *@param sContent The initial content of the new node.
	 *@returns A pointer to the newly created child node.
	 */
	XmlNode *addChild( const Bu::FString &sName );

	/**
	 * Add an already created XmlNode as a child to this node.  The new child
	 * XmlNode's parent will be changed appropriately and the parent XmlNode
	 * will take ownership of the child.
	 *@param pChild The child XmlNode to add to this XmlNode.
	 *@returns A pointer to the child node that was just added.
	 */
	XmlNode *addChild( XmlNode *pChild );

	/**
	 * Add a new property to the XmlNode.  Properties are name/value pairs.
	 *@param sName The name of the property.  Specifying a name that's already
	 * in use will overwrite that property.
	 *@param sValue The textual value of the property.
	 */
	void addProperty( const Bu::FString &sName, const Bu::FString &sValue );

	/**
	 * Get a pointer to the parent node, if any.
	 *@returns A pointer to the node's parent, or NULL if there isn't one.
	 */
	XmlNode *getParent();

	/**
	 * Tells you if this node has children.
	 *@returns True if this node has at least one child, false otherwise.
	 */
	bool hasChildren();

	/**
	 * Tells you how many children this node has.
	 *@returns The number of children this node has.
	 */
	int getNumChildren();

	/**
	 * Get a child with the specified name, and possibly skip value.  For an
	 * explination of skip values see the HashTable.
	 *@param sName The name of the child to find.
	 *@param nSkip The number of nodes with that name to skip.
	 *@returns A pointer to the child, or NULL if no child with that name was
	 * found.
	 */
	XmlNode *getChild( const Bu::FString &sName, int nSkip=0 );

	/**
	 * Get a pointer to the name of this node.  Do not change this, use setName
	 * instead.
	 *@returns A pointer to the name of this node.
	 */
	Bu::FString getName();

	/**
	 * Set the content of this node, optionally at a specific index.  Using the
	 * default of -1 will set the content after the last added node.
	 *@param sContent The content string to use.
	 *@param nIndex The index of the content.
	 */
	//void setContent( const char *sContent, int nIndex=-1 );

	/**
	 * Get the number of properties in this node.
	 *@returns The number of properties in this node.
	 */
	int getNumProperties();

	/**
	 * Get a propery's value by name.
	 *@param sName The name of the property to examine.
	 *@returns A pointer to the value of the property specified, or NULL if none
	 * found.
	 */
	Bu::FString getProperty( const Bu::FString &sName );

	/**
	 * Delete a child node, possibly replacing it with some text.  This actually
	 * fixes all content strings around the newly deleted child node.
	 *@param nIndex The index of the node to delete.
	 *@param sReplacementText The optional text to replace the node with.
	 *@returns True of the node was found, and deleted, false if it wasn't
	 * found.
	 */
	//void deleteNode( int nIndex, const char *sReplacementText = NULL );

	/**
	 * Delete a given node, but move all of it's children and content up to
	 * replace the deleted node.  All of the content of the child node is
	 * spliced seamlessly into place with the parent node's content.
	 *@param nIndex The node to delete.
	 *@returns True if the node was found and deleted, false if it wasn't.
	 */
	//void deleteNodeKeepChildren( int nIndex );

	/**
	 * Detatch a given child node from this node.  This effectively works just
	 * like a deleteNode, except that instead of deleting the node it is removed
	 * and returned, and all ownership is given up.
	 *@param nIndex The index of the node to detatch.
	 *@param sReplacementText The optional text to replace the detatched node
	 * with.
	 *@returns A pointer to the newly detatched node, which then passes
	 * ownership to the caller.
	 */
	//XmlNode *detatchNode( int nIndex, const char *sReplacementText = NULL );

	/**
	 * Replace a given node with a different node that is not currently owned by
	 * this XmlNode or any ancestor.
	 *@param nIndex The index of the node to replace.
	 *@param pNewNode The new node to replace the old node with.
	 *@returns True if the node was found and replaced, false if it wasn't.
	 */
	//void replaceNode( int nIndex, XmlNode *pNewNode );

	/**
	 * Replace a given node with the children and content of a given node.
	 *@param nIndex The index of the node to replace.
	 *@param pNewNode The node that contains the children and content that will
	 * replace the node specified by nIndex.
	 *@returns True if the node was found and replaced, false if it wasn't.
	 */
	//void replaceNodeWithChildren( int nIndex, XmlNode *pNewNode );

	/**
	 * Get a copy of this node and all children.  getCopy is recursive, so
	 * beware copying large trees of xml.
	 *@returns A newly created copy of this node and all of it's children.
	 */
	//XmlNode *getCopy();
	
	enum ChildType
	{
		typeNode,
		typeContent
	};

private:
	typedef struct
	{
		uint8_t nType;
		union {
			XmlNode *pNode;
			Bu::FString *pContent;
		};
	} Child;
	Bu::FString sName;	/**< The name of the node. */
	Bu::List<Child> lChildren;	/**< The children. */
	Bu::Hash<Bu::FString, Bu::FString> hProperties;	/**< Property hashtable. */
	Bu::Hash<Bu::FString, Bu::List<XmlNode *> > hChildren;	/**< Children hashtable. */
	XmlNode *pParent;	/**< A pointer to the parent of this node. */
	int nCurContent;	/**< The current content we're on, for using the -1 on
						 	 setContent. */
};

#endif