/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sha1.h"

Sha1::Sha1() :
	H0( 0x67452301 ),
	H1( 0xefcdab89 ),
	H2( 0x98badcfe ),
	H3( 0x10325476 ),
	H4( 0xc3d2e1f0 ),
	unprocessedBytes( 0 ),
	size( 0 )
{
}

Sha1::~Sha1()
{
}

void Sha1::process()
{
	int t;
	uint32_t a, b, c, d, e, K, f, W[80];

	// starting values
	a = H0;
	b = H1;
	c = H2;
	d = H3;
	e = H4;

	// copy and expand the message block
	for( t = 0; t < 16; t++ ) W[t] = (bytes[t*4] << 24)
									+(bytes[t*4 + 1] << 16)
									+(bytes[t*4 + 2] << 8)
									+ bytes[t*4 + 3];
	for(; t< 80; t++ ) W[t] = lrot( W[t-3]^W[t-8]^W[t-14]^W[t-16], 1 );
	
	/* main loop */
	uint32_t temp;
	for( t = 0; t < 80; t++ )
	{
		if( t < 20 ) {
			K = 0x5a827999;
			f = (b & c) | ((~b) & d);
		} else if( t < 40 ) {
			K = 0x6ed9eba1;
			f = b ^ c ^ d;
		} else if( t < 60 ) {
			K = 0x8f1bbcdc;
			f = (b & c) | (b & d) | (c & d);
		} else {
			K = 0xca62c1d6;
			f = b ^ c ^ d;
		}
		temp = lrot(a,5) + f + e + W[t] + K;
		e = d;
		d = c;
		c = lrot(b,30);
		b = a;
		a = temp;
		//printf( "t=%d %08x %08x %08x %08x %08x\n",t,a,b,c,d,e );
	}

	/* add variables */
	H0 += a;
	H1 += b;
	H2 += c;
	H3 += d;
	H4 += e;

	//printf( "Current: %08x %08x %08x %08x %08x\n",H0,H1,H2,H3,H4 );
	/* all bytes have been processed */
	unprocessedBytes = 0;
}

void Sha1::update( const char* data, int num )
{
	// add these bytes to the running total
	size += num;

	// repeat until all data is processed
	while( num > 0 )
	{
		// number of bytes required to complete block
		int needed = 64 - unprocessedBytes;

		// number of bytes to copy (use smaller of two)
		int toCopy = (num < needed) ? num : needed;

		// Copy the bytes
		memcpy( bytes + unprocessedBytes, data, toCopy );

		// Bytes have been copied
		num -= toCopy;
		data += toCopy;
		unprocessedBytes += toCopy;
		
		// there is a full block
		if( unprocessedBytes == 64 ) process();
	}
}

unsigned char* Sha1::getDigest()
{
	// save the message size
	uint32_t totalBitsL = size << 3;
	uint32_t totalBitsH = size >> 29;

	// add 0x80 to the message
	update( "\x80", 1 );
	
	unsigned char footer[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// block has no room for 8-byte filesize, so finish it
	if( unprocessedBytes > 56 )
		update( (char*)footer, 64 - unprocessedBytes);

	// how many zeros do we need
	int neededZeros = 56 - unprocessedBytes;

	// store file size (in bits) in big-endian format
	toBigEndian( totalBitsH, footer + neededZeros );
	toBigEndian( totalBitsL, footer + neededZeros + 4 );

	// finish the final block
	update( (char*)footer, neededZeros + 8 );

	// allocate memory for the digest bytes
	unsigned char* digest = new unsigned char[20];

	// copy the digest bytes
	toBigEndian( H0, digest );
	toBigEndian( H1, digest + 4 );
	toBigEndian( H2, digest + 8 );
	toBigEndian( H3, digest + 12 );
	toBigEndian( H4, digest + 16 );

	// return the digest
	return digest;
}

uint32_t Sha1::lrot( uint32_t x, int bits )
{
	return (x<<bits) | (x>>(32 - bits));
};

void Sha1::toBigEndian( uint32_t num, unsigned char* byte )
{
	byte[0] = (unsigned char)(num>>24);
	byte[1] = (unsigned char)(num>>16);
	byte[2] = (unsigned char)(num>>8);
	byte[3] = (unsigned char)num;
}

