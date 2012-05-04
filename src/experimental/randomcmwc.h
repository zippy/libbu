/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_CMWC_H
#define BU_RANDOM_CMWC_H

#include <stdint.h>

namespace Bu
{
	class RandomCmwc
	{
	public:
		RandomCmwc();
		virtual ~RandomCmwc();

		void seed( int32_t iSeed );

		int32_t rand();

	private:
		uint32_t *q, c;
	};
};

#endif
