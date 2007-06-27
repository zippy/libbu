#include "tafnode.h"

Bu::TafNode::TafNode( NodeType eType ) :
	eType( eType )
{
}

Bu::TafNode::~TafNode()
{
}

const Bu::TafNode::NodeType Bu::TafNode::getType() const
{
	return eType;
}

/*
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
*/

Bu::TafGroup::TafGroup( const Bu::FString &sName ) :
	TafNode( typeGroup ),
	sName( sName )
{
}

Bu::TafGroup::~TafGroup()
{
	//printf("Entering Bu::TafNode::~TafNode() \"%s\"\n", sName.getStr() );
	for( NodeList::iterator i = lChildren.begin(); i != lChildren.end(); i++ )
	{
		delete (*i);
	}
}

const Bu::FString &Bu::TafGroup::getName() const
{
	return sName;
}

Bu::TafNode *Bu::TafGroup::addChild( Bu::TafNode *pNode )
{
	switch( pNode->getType() )
	{
		case typeGroup:
			{
				TafGroup *pGroup = (TafGroup *)pNode;
				if( !hChildren.has( pGroup->getName() ) )
					hChildren.insert( pGroup->getName(), GroupList() );
				hChildren.get( pGroup->getName() ).append( pGroup );
			}
			break;

		case typeProperty:
			{
				TafProperty *pProperty = (TafProperty *)pNode;
				if( !hProp.has( pProperty->getName() ) )
					hProp.insert( pProperty->getName(), PropList() );
				hProp.get( pProperty->getName() ).append( pProperty->getValue() );
			}
			break;

		case typeComment:
			break;
	}

	lChildren.append( pNode );

	return pNode;
}

const Bu::TafGroup::GroupList &Bu::TafGroup::getChildren( const Bu::FString &sName ) const
{
	return hChildren.get( sName );
}

const Bu::TafGroup::NodeList &Bu::TafGroup::getChildren() const
{
	return lChildren;
}

const Bu::TafGroup *Bu::TafGroup::getChild( const Bu::FString &sName ) const
{
	return hChildren.get( sName ).first();
}

const Bu::TafGroup::PropList &Bu::TafGroup::getProperties( const Bu::FString &sName ) const
{
	return hProp.get( sName );
}

const Bu::FString &Bu::TafGroup::getProperty( const Bu::FString &sName ) const
{
	return hProp.get( sName ).first();
}

Bu::TafProperty::TafProperty( const Bu::FString &sName, const Bu::FString &sValue ) :
	TafNode( typeProperty ),
	sName( sName ),
	sValue( sValue )
{
}

Bu::TafProperty::~TafProperty()
{
}

const Bu::FString &Bu::TafProperty::getName() const
{
	return sName;
}

const Bu::FString &Bu::TafProperty::getValue() const
{
	return sValue;
}

Bu::TafComment::TafComment( const Bu::FString &sText ) :
	TafNode( typeComment ),
	sText( sText )
{
}

Bu::TafComment::~TafComment()
{
}

const Bu::FString &Bu::TafComment::getText() const
{
	return sText;
}

