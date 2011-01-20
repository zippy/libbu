/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CONDUIT_H
#define BU_CONDUIT_H

#include "bu/stream.h"
#include "bu/string.h"

namespace Bu
{
	/**
	 * Simple inter-thread communication stream.  This acts like a pair of
	 * pipes for stream communication between any two things, but without the
	 * use of pipes, making this a bad choice for IPC.
	 */
	class Conduit : public Stream
	{
	};
}

#endif
