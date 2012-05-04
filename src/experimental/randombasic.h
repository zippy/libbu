/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_BASIC_H
#define BU_RANDOM_BASIC_H

#include <stdint.h>

namespace Bu
{
	class RandomBasic
	{
	public:
		RandomBasic();
		virtual ~RandomBasic();

	private:
		int64_t c, m, a;
	};
};

#endif
