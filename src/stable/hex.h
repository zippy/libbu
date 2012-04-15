/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_HEX_H
#define BU_HEX_H

#include "bu/filter.h"

namespace Bu
{
	/**
	 * This very simple filter encodes to/decodes from hex encoded string data.
	 * The primary use of this filter is in debugging, use it with
	 * Bu::encodeStr to easily create hex dumps of string data, even other raw
	 * structures.
	 *
	 *@code
	 Bu::println("Hexdump: " + Bu::encodeStr<Bu::Hex>("Test data ;)") );
	 @endcode
	 * Or...
	 *@code
	 complex_struct data;
	 ...
	 Bu::println("Hexdump: " +
	 	Bu::encodeStr<Bu::Hex>(
	 		Bu::String( &data, sizeof(data) )
			)
		);
	 @endcode
	 **/
	class Hex : public Bu::Filter
	{
	public:
		Hex( Bu::Stream &rNext, bool bUpperCase=false, int iChunk=-1 );
		virtual ~Hex();

		virtual void start();
		virtual Bu::size stop();

		virtual Bu::size read( void *pBuf, Bu::size iBytes );
		virtual Bu::size write( const void *pBuf, Bu::size iBytes );
		using Bu::Stream::write;

	private:
		int iChunk;
		Bu::size iPos;
		char cIn[2];
		int iIn;
		const char *sChrs;
	};
};

#endif
