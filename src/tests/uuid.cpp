/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/uuid.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	Uuid i = Uuid::gen();

	sio << i.toString() << sio.nl;

	return 0;
}

