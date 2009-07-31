/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/uuid.h"

Bu::Uuid::Uuid()
{
	clear();
}

Bu::Uuid::~Uuid()
{
}

#define msb( i ) (1<<(7-i))

void Bu::Uuid::clear()
{
	data[7] = msb(0);
}

