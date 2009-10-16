/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafgroup.h"
#include "bu/tafproperty.h"
#include "bu/tafcomment.h"

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

bool Bu::TafGroup::hasChild( const Bu::FString &sName ) const
{
	return hChildren.has( sName );
}

const Bu::TafGroup::GroupList &Bu::TafGroup::getChildren( const Bu::FString &sName ) const
{
	try {
		return hChildren.get( sName );
	} catch( Bu::HashException &e )
	{
		throw Bu::TafException("No children of group \"%s\" match \"%s\".",
			this->sName.getStr(), sName.getStr() );
	}
}

const Bu::TafGroup::NodeList &Bu::TafGroup::getChildren() const
{
	return lChildren;
}

const Bu::TafGroup *Bu::TafGroup::getChild( const Bu::FString &sName ) const
{
	try {
		return hChildren.get( sName ).first();
	} catch( Bu::HashException &e )
	{
		throw Bu::TafException("No children of group \"%s\" match \"%s\".",
			this->sName.getStr(), sName.getStr() );
	}
}

bool Bu::TafGroup::hasProperty( const Bu::FString &sName ) const
{
	return hProp.has( sName );
}

const Bu::TafGroup::PropList &Bu::TafGroup::getProperties( const Bu::FString &sName ) const
{
	try {
		return hProp.get( sName );
	} catch( Bu::HashException &e )
	{
		throw Bu::TafException("No properties of group \"%s\" match \"%s\".",
			this->sName.getStr(), sName.getStr() );
	}
}

const Bu::FString &Bu::TafGroup::getProperty( const Bu::FString &sName ) const
{
	try {
		return hProp.get( sName ).first();
	} catch( Bu::HashException &e )
	{
		throw Bu::TafException("No properties of group \"%s\" match \"%s\".",
			this->sName.getStr(), sName.getStr() );
	}	
}

const Bu::FString &Bu::TafGroup::getProperty( const Bu::FString &sName,
	const Bu::FString &sDef ) const
{
	try
	{
		return hProp.get( sName ).first();
	}
	catch( Bu::HashException &e )
	{
		return sDef;
	}
}

