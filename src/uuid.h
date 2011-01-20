/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UUID_H
#define BU_UUID_H

#include "bu/string.h"

namespace Bu
{
	class Uuid
	{
	public:
		Uuid();
		Uuid( const Uuid &src );
		virtual ~Uuid();

		Bu::String toRawString();
		Bu::String toString();

		static Uuid gen();
		static Uuid genV1();
		static Uuid genV2();
		static Uuid genV3();
		static Uuid genV4();
		static Uuid genV5();

		void clear();

	private:
		unsigned char data[16];
	};
};

#endif
