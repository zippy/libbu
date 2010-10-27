/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/variant.h"

namespace Bu
{
	Formatter &operator<<( Formatter &f, const FString &s );
};

Bu::VariantTypeRoot::VariantTypeRoot()
{
}

Bu::VariantTypeRoot::~VariantTypeRoot()
{
}

Bu::Variant::Variant() :
	pCore( NULL )
{
}

Bu::Variant::Variant( const Variant &v ) :
	pCore( NULL )
{
	if( v.pCore )
	{
		pCore = v.pCore->clone();
	}
}

Bu::Variant::Variant( const char *t ) :
	pCore( NULL )
{
	set( Bu::FString( t ) );
}

Bu::Variant::~Variant()
{
	if( pCore )
	{
		delete pCore;
		pCore = NULL;
	}
}

bool Bu::Variant::isSet() const
{
	return pCore != NULL;
}

Bu::FString Bu::Variant::toString() const
{
	if( !pCore )
		return "***NO DATA***";
	return pCore->toString();
}

const std::type_info &Bu::Variant::getType() const
{
	if( !pCore )
	{
		throw Bu::ExceptionBase("No data!");
	}
	return pCore->getType();
}

Bu::Variant &Bu::Variant::operator=( const Bu::Variant &rhs )
{
	if( pCore )
	{
		delete pCore;
		pCore = NULL;
	}
	if( rhs.pCore )
	{
		pCore = rhs.pCore->clone();
	}

	return *this;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::Variant &v )
{
	return f << v.toString();
}

template<> Bu::FString Bu::VariantType<int>::toString() const
{
	Bu::FString s;
	s.format("%d", data );
	return s;
}

template<> Bu::FString Bu::VariantType<bool>::toString() const
{
	return data?"true":"false";
}

