/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UTF_STRING_H
#define BU_UTF_STRING_H

#include <stdint.h>

namespace Bu
{
	class String;

	class UtfString
	{
	public:
		UtfString();
		virtual ~UtfString();

		typedef uint32_t point;

		static void debugUtf8( const Bu::String &sUtf8 );

	private:
//		typedef BasicString<uint16_t> RawString;
//		RawString rsStore;
	};
};

#endif
