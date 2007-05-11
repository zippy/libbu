#include "xmlnode.h"

XmlNode::XmlNode( const Bu::FString &sName, XmlNode *pParent ) :
	sName( sName ),
	pParent( pParent )
{
}

XmlNode::~XmlNode()
{
}
/*
void XmlNode::setName( const char *sName )
{
	if( pParent )
	{
		if( this->sName.size() == 0 )
		{
			// We're not in the hash yet, so add us
			this->sName = sName;
			pParent->hChildren.insert( this->sName.c_str(), this );
		}
		else
		{
			// Slightly more tricky, delete us, then add us...
			pParent->hChildren.del( this->sName.c_str() );
			this->sName = sName;
			pParent->hChildren.insert( this->sName.c_str(), this );
		}
	}
	else
	{
		// If we have no parent, then just set the name string, we don't need
		// to worry about hashing.
		this->sName = sName;
	}
}

void XmlNode::setContent( const char *sContent, int nIndex )
{
	if( nIndex == -1 )
	{
		nIndex = nCurContent;
	}
	if( nIndex == 0 )
	{
		if( this->sPreContent )
		{
			delete this->sPreContent;
		}
	
		this->sPreContent = new std::string( sContent );
	}
	else
	{
		nIndex--;
		if( lPostContent[nIndex] )
		{
			delete (std::string *)lPostContent[nIndex];
		}

		lPostContent.setAt( nIndex, new std::string( sContent ) );
	}
}

const char *XmlNode::getContent( int nIndex )
{
	if( nIndex == 0 )
	{
		if( sPreContent )
		{
			return sPreContent->c_str();
		}
	}
	else
	{
		nIndex--;
		if( lPostContent[nIndex] )
		{
			return ((std::string *)lPostContent[nIndex])->c_str();
		}
	}

	return NULL;
}*/

XmlNode *XmlNode::addChild( const Bu::FString &sName )
{
	return addChild( new XmlNode( sName, this ) );
}

XmlNode *XmlNode::addChild( XmlNode *pNode )
{
	Child c = { typeNode };
	c.pNode = pNode;
	lChildren.append( c );
	pNode->pParent = this;

	return pNode;
}

XmlNode *XmlNode::getParent()
{
	return pParent;
}

void XmlNode::addProperty( const Bu::FString &sName, const Bu::FString &sValue )
{
	hProperties.insert( sName, sValue );
}

int XmlNode::getNumProperties()
{
	return hProperties.size();
}
/*
const char *XmlNode::getPropertyName( int nIndex )
{
	std::string *tmp = ((std::string *)lPropNames[nIndex]);
	if( tmp == NULL )
		return NULL;
	return tmp->c_str();
}

const char *XmlNode::getProperty( int nIndex )
{
	std::string *tmp = ((std::string *)lPropValues[nIndex]);
	if( tmp == NULL )
		return NULL;
	return tmp->c_str();
}
*/
Bu::FString XmlNode::getProperty( const Bu::FString &sName )
{
	return hProperties[sName];
}
/*
void XmlNode::deleteProperty( int nIndex )
{
	hProperties.del( ((std::string *)lPropNames[nIndex])->c_str() );

	delete (std::string *)lPropNames[nIndex];
	delete (std::string *)lPropValues[nIndex];

	lPropNames.deleteAt( nIndex );
	lPropValues.deleteAt( nIndex );
}

bool XmlNode::hasChildren()
{
	return hChildren.getSize()>0;
}*/

int XmlNode::getNumChildren()
{
	return lChildren.getSize();
}
/*
XmlNode *XmlNode::getChild( int nIndex )
{
	return (XmlNode *)lChildren[nIndex];
}
*/
XmlNode *XmlNode::getChild( const Bu::FString &sName, int nSkip )
{
	if( !hChildren.has( sName ) )
		return NULL;

	Bu::List<XmlNode *>::iterator i = hChildren[sName]->begin();
	return *i;
}

Bu::FString XmlNode::getName()
{
	return sName;
}
/*
void XmlNode::deleteNode( int nIndex, const char *sReplacementText )
{
	XmlNode *xRet = detatchNode( nIndex, sReplacementText );

	if( xRet != NULL )
	{
		delete xRet;
	}
}

XmlNode *XmlNode::detatchNode( int nIndex, const char *sReplacementText )
{
	if( nIndex < 0 || nIndex >= lChildren.getSize() )
		return NULL;
	
	// The real trick when deleteing a node isn't actually deleting it, it's
	// reforming the content around the node that's now missing...hmmm...
	
	if( nIndex == 0 )
	{
		// If the index is zero we have to deal with the pre-content
		if( sReplacementText )
		{
			if( sPreContent == NULL )
			{
				sPreContent = new std::string( sReplacementText );
			}
			else
			{
				*sPreContent += sReplacementText;
			}
		}
		if( lPostContent.getSize() > 0 )
		{
			if( lPostContent[0] != NULL )
			{
				if( sPreContent == NULL )
				{
					sPreContent = new std::string(
						((std::string *)lPostContent[0])->c_str()
						);
				}
				else
				{
					*sPreContent +=
						((std::string *)lPostContent[0])->c_str();
				}
			}
			delete (std::string *)lPostContent[0];
			lPostContent.deleteAt( 0 );
		}
	}
	else
	{
		int nCont = nIndex-1;
		// If it's above zero we deal with the post-content only
		if( sReplacementText )
		{
			if( lPostContent[nCont] == NULL )
			{
				lPostContent.setAt( nCont, new std::string( sReplacementText ) );
			}
			else
			{
				*((std::string *)lPostContent[nCont]) += sReplacementText;
			}
		}
		if( lPostContent.getSize() > nIndex )
		{
			if( lPostContent[nIndex] != NULL )
			{
				if( lPostContent[nCont] == NULL )
				{
					lPostContent.setAt( nCont, new std::string(
						((std::string *)lPostContent[nIndex])->c_str()
						) );
				}
				else
				{
					*((std::string *)lPostContent[nCont]) +=
						((std::string *)lPostContent[nIndex])->c_str();
				}
			}
			delete (std::string *)lPostContent[nIndex];
			lPostContent.deleteAt( nIndex );
		}
	}

	XmlNode *xRet = (XmlNode *)lChildren[nIndex];
	hChildren.del( ((XmlNode *)lChildren[nIndex])->getName() );
	lChildren.deleteAt( nIndex );
	
	return xRet;
}

void XmlNode::replaceNode( int nIndex, XmlNode *pNewNode )
{
	if( nIndex < 0 || nIndex >= lChildren.getSize() )
		return; //TODO: throw an exception
	
	delete (XmlNode *)lChildren[nIndex];
	lChildren.setAt( nIndex, pNewNode );
	pNewNode->pParent = this;
}

XmlNode *XmlNode::getCopy()
{
	XmlNode *pNew = new XmlNode();

	pNew->sName = sName;
	if( sPreContent )
	{
		pNew->sPreContent = new std::string( sPreContent->c_str() );
	}
	else
	{
		pNew->sPreContent = NULL;
	}
	pNew->nCurContent = 0;
	
	int nSize = lPostContent.getSize();
	pNew->lPostContent.setSize( nSize );
	for( int j = 0; j < nSize; j++ )
	{
		if( lPostContent[j] )
		{
			pNew->lPostContent.setAt(
				j, new std::string(
					((std::string *)lPostContent[j])->c_str()
					)
				);
		}
		else
		{
			pNew->lPostContent.setAt( j, NULL );
		}
	}

	nSize = lChildren.getSize();
	pNew->lChildren.setSize( nSize );
	for( int j = 0; j < nSize; j++ )
	{
		XmlNode *pChild = ((XmlNode *)lChildren[j])->getCopy();
		pNew->lChildren.setAt( j, pChild );
		pChild->pParent = pNew;
		pNew->hChildren.insert( pChild->getName(), pChild );
	}

	nSize = lPropNames.getSize();
	pNew->lPropNames.setSize( nSize );
	pNew->lPropValues.setSize( nSize );
	for( int j = 0; j < nSize; j++ )
	{
		std::string *pProp = new std::string( ((std::string *)lPropNames[j])->c_str() );
		std::string *pVal = new std::string( ((std::string *)lPropValues[j])->c_str() );
		pNew->lPropNames.setAt( j, pProp );
		pNew->lPropValues.setAt( j, pVal );
		pNew->hProperties.insert( pProp->c_str(), pVal->c_str() );
		pNew->nCurContent++;
	}

	return pNew;
}

void XmlNode::deleteNodeKeepChildren( int nIndex )
{
	// This is a tricky one...we need to do some patching to keep things all
	// even...
	XmlNode *xRet = (XmlNode *)lChildren[nIndex];

	if( xRet == NULL )
	{
		return;
	}
	else
	{
		if( getContent( nIndex ) )
		{
			std::string sBuf( getContent( nIndex ) );
			sBuf += xRet->getContent( 0 );
			setContent( sBuf.c_str(), nIndex );
		}
		else
		{
			setContent( xRet->getContent( 0 ), nIndex );
		}
		
		int nSize = xRet->lChildren.getSize();
		for( int j = 0; j < nSize; j++ )
		{
			XmlNode *pCopy = ((XmlNode *)xRet->lChildren[j])->getCopy();
			pCopy->pParent = this;
			lChildren.insertBefore( pCopy, nIndex+j );
		
			if( xRet->lPostContent[j] )
			{
				lPostContent.insertBefore(
					new std::string( ((std::string *)xRet->lPostContent[j])->c_str() ),
					nIndex+j
					);
			}
			else
			{
				lPostContent.insertBefore( NULL, nIndex+j );
			}
		}
		
		if( getContent( nIndex+nSize ) )
		{
			//SString sBuf( getContent( nIndex+nSize ) );
			//sBuf.catfrom( xRet->getContent( nSize ) );
			//setContent( sBuf, nIndex+nSize );
		}
		else
		{
			setContent( xRet->getContent( nSize ), nIndex+nSize );
		}

		deleteNode( nIndex+nSize );
	}
}

void XmlNode::replaceNodeWithChildren( int nIndex, XmlNode *pNewNode )
{
}
*/
