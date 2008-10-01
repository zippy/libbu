/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafnode.h"

namespace Bu { subExceptionDef( TafException ) }

Bu::TafNode::TafNode( NodeType eType ) :
	eType( eType )
{
}

Bu::TafNode::~TafNode()
{
}

Bu::TafNode::NodeType Bu::TafNode::getType() const
{
	return eType;
}

Bu::TafGroup::TafGroup( const Bu::FString &sName ) :
	TafNode( typeGroup ),
	sName( sName )
{
}

Bu::TafGroup::~TafGroup()
{
	for( NodeList::iterator i = lChildren.begin(); i != lChildren.end(); i++ )
	{
		delete (*i);
	}
}

const Bu::FString &Bu::TafGroup::getName() const
{
	return sName;
}

void Bu::TafGroup::setName( const Bu::FString &sName )
{
	this->sName = sName;
}

Bu::TafNode *Bu::TafGroup::addChild( Bu::TafNode *pNode )
{
	switch( pNode->getType() )
	{
		case typeGroup:
			addChild( (Bu::TafGroup *)pNode );
			break;

		case typeProperty:
			addChild( (Bu::TafProperty *)pNode );
			break;

		case typeComment:
			addChild( (Bu::TafComment *)pNode );
			break;
	}

	return pNode;
}

Bu::TafGroup *Bu::TafGroup::addChild( TafGroup *pNode )
{
	TafGroup *pGroup = (TafGroup *)pNode;
	if( !hChildren.has( pGroup->getName() ) )
		hChildren.insert( pGroup->getName(), GroupList() );
	hChildren.get( pGroup->getName() ).append( pGroup );
	lChildren.append( pNode );
	return pNode;
}

Bu::TafProperty *Bu::TafGroup::addChild( TafProperty *pNode )
{
	TafProperty *pProperty = (TafProperty *)pNode;
	if( !hProp.has( pProperty->getName() ) )
		hProp.insert( pProperty->getName(), PropList() );
	hProp.get( pProperty->getName() ).append( pProperty->getValue() );
	lChildren.append( pNode );
	return pNode;
}

Bu::TafComment *Bu::TafGroup::addChild( TafComment *pNode )
{
	lChildren.append( pNode );
	return pNode;
}

Bu::TafGroup *Bu::TafGroup::addGroup( const Bu::FString &sName )
{
	return addChild( new TafGroup( sName ) );
}

Bu::TafProperty *Bu::TafGroup::addProperty(
	const Bu::FString &sName, const Bu::FString &sValue )
{
	return addChild( new TafProperty( sName, sValue ) );
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

const Bu::FString &Bu::TafGroup::getProperty( const Bu::FString &sName,
	const Bu::FString &sDef ) const
{
	try
	{
		return getProperty( sName );
	}
	catch( Bu::HashException &e )
	{
		return sDef;
	}
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

Bu::TafComment::TafComment( const Bu::FString &sText, bool bEOL ) :
	TafNode( typeComment ),
	sText( sText ),
	bEOL( bEOL )
{
}

Bu::TafComment::~TafComment()
{
}

const Bu::FString &Bu::TafComment::getText() const
{
	return sText;
}

bool Bu::TafComment::isEOLStyle() const
{
	return bEOL;
}

