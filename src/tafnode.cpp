#include "tafnode.h"

Bu::TafNode::TafNode()
{
}

Bu::TafNode::~TafNode()
{
	printf("Entering Bu::TafNode::~TafNode() \"%s\"\n", sName.getStr() );
	for( NodeHash::iterator i = hChildren.begin(); i != hChildren.end(); i++ )
	{
		NodeList &l = i.getValue();
		for( NodeList::iterator k = l.begin(); k != l.end(); k++ )
		{
			printf("deleting:  [%08X] %s\n", *k, "" );//(*k)->getName().getStr() );
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

	printf("Appending \"%s\"\n", pNode->getName().getStr() );
	hChildren.get( pNode->getName() ).append( pNode );
	printf("[%08X]\n", hChildren.get( pNode->getName() ).last() );
}

const Bu::TafNode::PropList &Bu::TafNode::getProperty( const Bu::FString &sName )
{
	return hProp.get( sName );
}

const Bu::TafNode::NodeList &Bu::TafNode::getNode( const Bu::FString &sName )
{
	return hChildren.get( sName );
}

void Bu::TafNode::setName( const Bu::FString &sName )
{
	this->sName = sName;
}

const Bu::FString &Bu::TafNode::getName()
{
	return sName;
}

