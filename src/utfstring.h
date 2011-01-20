/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UTF_STRING_H
#define BU_UTF_STRING_H

#include "bu/string.h"

namespace Bu
{
	class UtfString
	{
	public:
		UtfString();
		virtual ~UtfString();

		typedef uint32_t point;

	private:
		typedef BasicString<uint16_t> RawString;
		RawString rsStore;
	};
};

#endif
