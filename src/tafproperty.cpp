/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafproperty.h"

Bu::TafProperty::TafProperty( const Bu::TafProperty &rSrc ) :
	TafNode( typeProperty ),
	sName( rSrc.sName ),
	sValue( rSrc.sValue )
{
}

Bu::TafProperty::TafProperty( const Bu::String &sName, const Bu::String &sValue ) :
	TafNode( typeProperty ),
	sName( sName ),
	sValue( sValue )
{
}

Bu::TafProperty::~TafProperty()
{
}

const Bu::String &Bu::TafProperty::getName() const
{
	return sName;
}

const Bu::String &Bu::TafProperty::getValue() const
{
	return sValue;
}

