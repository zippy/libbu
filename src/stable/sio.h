/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SIO_H
#define BU_SIO_H

#include "bu/stdstream.h"
#include "bu/formatter.h"

namespace Bu
{
	extern Bu::StdStream sioRaw;
	extern Bu::Formatter sio;

	Bu::String::FormatProxy print( Bu::Stream &s, const Bu::String &str );
	Bu::String::FormatProxy print( const Bu::String &str );

	Bu::String::FormatProxy println( Bu::Stream &s, const Bu::String &str );
	Bu::String::FormatProxy println( const Bu::String &str );
};

#endif
