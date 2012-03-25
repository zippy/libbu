/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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

	Bu::size print( Bu::Stream &s, const Bu::String &str );
	Bu::size print( const Bu::String &str );

	Bu::size println( Bu::Stream &s, const Bu::String &str );
	Bu::size println( const Bu::String &str );
};

#endif
