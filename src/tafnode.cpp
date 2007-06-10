#include "tafnode.h"

Bu::TafNode::TafNode()
{
}

Bu::TafNode::~TafNode()
{
	//printf("Entering Bu::TafNode::~TafNode() \"%s\"\n", sName.getStr() );
	for( NodeHash::iterator i = hChildren.begin(); i != hChildren.end(); i++ )
	{
		NodeList &l = i.getValue();
		for( NodeList::iterator k = l.begin(); k != l.end(); k++ )
		{
			//printf("deleting:  [%08X] %s\n", *k, "" );//(*k)->getName().getStr() );
			delete (*k);
		}
	}
}

void Bu::TafNode::setProperty( Bu::FString sName, Bu::FString sValue )
{
	if( !hProp.has( sName ) )
	{
		hProp.insert( sName, PropList() );
	}

	hProp.get( sName ).append( sValue );
}

void Bu::TafNode::addChild( TafNode *pNode )
{
	if( !hChildren.has( pNode->getName() ) )
	{
		hChildren.insert( pNode->getName(), NodeList() );
	}

	//printf("Appending \"%s\"\n", pNode->getName().getStr() );
	hChildren.get( pNode->getName() ).append( pNode );
	//printf("[%08X]\n", hChildren.get( pNode->getName() ).last() );
}

const Bu::TafNode::PropList &Bu::TafNode::getProperties( const Bu::FString &sName ) const
{
	return hProp.get( sName );
}

const Bu::TafNode::NodeList &Bu::TafNode::getChildren( const Bu::FString &sName ) const
{
	return hChildren.get( sName );
}

const Bu::FString &Bu::TafNode::getProperty( const Bu::FString &sName ) const
{
	return getProperties( sName ).first();
}

const Bu::TafNode *Bu::TafNode::getChild( const Bu::FString &sName ) const
{
	return getChildren( sName ).first();
}

void Bu::TafNode::setName( const Bu::FString &sName )
{
	this->sName = sName;
}

const Bu::FString &Bu::TafNode::getName() const
{
	return sName;
}

