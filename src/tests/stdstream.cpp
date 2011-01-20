/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"

using Bu::sio;
using Bu::Fmt;

int main()
{
	sio << "Hello there" << sio.nl;

	sio << "sizeof(Fmt) = " << sizeof(Fmt) << sio.nl;

	sio << -123 << ", " << 0 << ", " << 123 << sio.nl;

	sio << "+----------+" << sio.nl;
	sio << "|" << Fmt( 10, 10, Fmt::Center ) << "Test" << "|" << sio.nl;
	sio << "+----------+" << sio.nl;
	sio << "|" << Fmt( 10, 10, Fmt::Left ) << 123 << "|" << sio.nl;
	sio << "|" << Fmt( 10, 10, Fmt::Center ) << 123 << "|" << sio.nl;
	sio << "|" << Fmt( 10, 10, Fmt::Right ) << 123 << "|" << sio.nl;
	sio << "+----------+" << sio.nl;

	sio << Fmt(10,Fmt::Left) << "Hexcode:" << Fmt::ptr() << (void*)(&sio) << sio.nl;

	sio << 0.123 << sio.nl;
	sio << true << " and then " << false << sio.nl;

	for( int j = 2; j <= 36; j++ )
		sio << "radix(" << j << ") = " << Fmt().radix( j ).width( 8 ).align( Fmt::Right ) << 255 << sio.nl;

	return 0;
}

