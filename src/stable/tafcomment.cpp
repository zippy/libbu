/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafcomment.h"

Bu::TafComment::TafComment( const Bu::TafComment &rSrc ) :
    TafNode( typeComment ),
    sText( rSrc.sText ),
    bEOL( rSrc.bEOL )
{
}

Bu::TafComment::TafComment( const Bu::String &sText, bool bEOL ) :
    TafNode( typeComment ),
    sText( sText ),
    bEOL( bEOL )
{
}

Bu::TafComment::~TafComment()
{
}

const Bu::String &Bu::TafComment::getText() const
{
    return sText;
}

bool Bu::TafComment::isEOLStyle() const
{
    return bEOL;
}

