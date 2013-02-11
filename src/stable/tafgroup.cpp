/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafgroup.h"
#include "bu/tafproperty.h"
#include "bu/tafcomment.h"

Bu::TafGroup::TafGroup( const TafGroup &rSrc ) :
    TafNode( typeGroup ),
    sName( rSrc.sName )
{
    for( NodeList::const_iterator i = rSrc.lChildren.begin(); i; i++ )
    {
        switch( (*i)->getType() )
        {
            case typeGroup:
                addChild( new TafGroup( *dynamic_cast<const TafGroup *>(*i) ) );
                break;

            case typeProperty:
                addChild( new TafProperty( *dynamic_cast<const TafProperty *>(*i) ) );
                break;

            case typeComment:
                addChild( new TafComment( *dynamic_cast<const TafComment *>(*i) ) );
                break;
        }
    }
}

Bu::TafGroup::TafGroup( const Bu::String &sName ) :
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

const Bu::String &Bu::TafGroup::getName() const
{
    return sName;
}

void Bu::TafGroup::setName( const Bu::String &sName )
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

Bu::TafGroup *Bu::TafGroup::addGroup( const Bu::String &sName )
{
    return addChild( new TafGroup( sName ) );
}

Bu::TafProperty *Bu::TafGroup::addProperty(
    const Bu::String &sName, const Bu::String &sValue )
{
    return addChild( new TafProperty( sName, sValue ) );
}

bool Bu::TafGroup::hasChild( const Bu::String &sName ) const
{
    return hChildren.has( sName );
}

const Bu::TafGroup::GroupList &Bu::TafGroup::getChildren( const Bu::String &sName ) const
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

const Bu::TafGroup *Bu::TafGroup::getChild( const Bu::String &sName ) const
{
    try {
        return hChildren.get( sName ).first();
    } catch( Bu::HashException &e )
    {
        throw Bu::TafException("No children of group \"%s\" match \"%s\".",
            this->sName.getStr(), sName.getStr() );
    }
}

bool Bu::TafGroup::hasProperty( const Bu::String &sName ) const
{
    return hProp.has( sName );
}

const Bu::TafGroup::PropList &Bu::TafGroup::getProperties( const Bu::String &sName ) const
{
    try {
        return hProp.get( sName );
    } catch( Bu::HashException &e )
    {
        throw Bu::TafException("No properties of group \"%s\" match \"%s\".",
            this->sName.getStr(), sName.getStr() );
    }
}

const Bu::String &Bu::TafGroup::getProperty( const Bu::String &sName ) const
{
    try {
        return hProp.get( sName ).first();
    } catch( Bu::HashException &e )
    {
        throw Bu::TafException("No properties of group \"%s\" match \"%s\".",
            this->sName.getStr(), sName.getStr() );
    }   
}

const Bu::String &Bu::TafGroup::getProperty( const Bu::String &sName,
    const Bu::String &sDef ) const
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

const Bu::TafGroup *Bu::TafGroup::getChildByPath(
        const Bu::String &sPath ) const
{
    return getChildByPath( sPath.split('/') );
}

const Bu::TafGroup *Bu::TafGroup::getChildByPath( Bu::StrList lPath ) const
{
    const Bu::TafGroup *cur = this;

    for( Bu::StrList::const_iterator i = lPath.begin(); i; i++ )
    {
        cur = cur->getChild( *i );
    }

    return cur;
}

const Bu::String &Bu::TafGroup::getByPath( const Bu::String &sPath ) const
{
    return getByPath( sPath.split('/') );
}

const Bu::String &Bu::TafGroup::getByPath( Bu::StrList lPath ) const
{
    const Bu::TafGroup *cur = this;

    for( Bu::StrList::const_iterator i = lPath.begin(); i; i++ )
    {
        if( !(i+1) )
            break;
        cur = cur->getChild( *i );
    }

    return cur->getProperty( lPath.last() );
}

