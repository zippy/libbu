/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
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

