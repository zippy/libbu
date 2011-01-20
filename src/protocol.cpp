/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/protocol.h"

using namespace Bu;

Bu::Protocol::Protocol()
{
}

Bu::Protocol::~Protocol()
{
}

void Bu::Protocol::onNewConnection( Bu::Client * )
{
}

void Bu::Protocol::onNewData( Bu::Client * )
{
}

void Bu::Protocol::onMessage( Bu::Client *, const Bu::String & )
{
}

void Bu::Protocol::onTick( Bu::Client * )
{
}

