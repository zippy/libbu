/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

/* sha1.h

Copyright (c) 2005 Michael D. Leonhard

http://tamale.net/

This file is licensed under the terms described in the
accompanying LICENSE file.
*/

#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>

/**
 * Calculates SHA-1 sums.  This is based strongly on code from Michael D.
 * Leonhard who released his code under the terms of the MIT license, thank you!
 */
class Sha1
{
public:
	Sha1();
	~Sha1();

	void update( const char* data, int num );
	unsigned char* getDigest();

	// utility methods
	static uint32_t lrot( uint32_t x, int bits );
	static void toBigEndian( uint32_t in, unsigned char* out );

private:
	// fields
	uint32_t H0, H1, H2, H3, H4;
	unsigned char bytes[64];
	int unprocessedBytes;
	uint32_t size;
	void process();
};

#endif
