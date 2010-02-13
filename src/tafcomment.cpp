/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafcomment.h"

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

