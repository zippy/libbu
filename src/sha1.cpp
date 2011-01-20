/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <string.h>

#include "bu/stream.h"

#include "bu/sha1.h"

Bu::Sha1::Sha1() :
	uH0( 0x67452301 ),
	uH1( 0xefcdab89 ),
	uH2( 0x98badcfe ),
	uH3( 0x10325476 ),
	uH4( 0xc3d2e1f0 ),
	iUnprocessedBytes( 0 ),
	uTotalBytes( 0 )
{
	reset();
}

Bu::Sha1::~Sha1()
{
}

void Bu::Sha1::reset()
{
	uH0 = 0x67452301;
	uH1 = 0xefcdab89;
	uH2 = 0x98badcfe;
	uH3 = 0x10325476;
	uH4 = 0xc3d2e1f0;
	iUnprocessedBytes = 0;
	uTotalBytes = 0;
}

void Bu::Sha1::setSalt( const Bu::String & /*sSalt*/ )
{
}

void Bu::Sha1::addData( const void *sDataRaw, int iSize )
{
	const unsigned char *sData = (const unsigned char *)sDataRaw;
	// add these bytes to the running total
	uTotalBytes += iSize;

	// repeat until all data is processed
	while( iSize > 0 )
	{
		// number of bytes required to complete block
		int iNeeded = 64 - iUnprocessedBytes;

		// number of bytes to copy (use smaller of two)
		int iToCopy = (iSize < iNeeded) ? iSize : iNeeded;

		// Copy the bytes
		memcpy( uBytes + iUnprocessedBytes, sData, iToCopy );

		// Bytes have been copied
		iSize -= iToCopy;
		sData += iToCopy;
		iUnprocessedBytes += iToCopy;
		
		// there is a full block
		if( iUnprocessedBytes == 64 )
		{
			process();
		}
	}
}

Bu::String Bu::Sha1::getResult()
{
	// save the message size
	uint32_t totalBitsL = uTotalBytes << 3;
	uint32_t totalBitsH = uTotalBytes >> 29;

	// add 0x80 to the message
	addData( "\x80", 1 );
	
	unsigned char footer[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// block has no room for 8-byte filesize, so finish it
	if( iUnprocessedBytes > 56 )
		addData( (char*)footer, 64 - iUnprocessedBytes);

	// how many zeros do we need
	int iNeededZeros = 56 - iUnprocessedBytes;

	// store file size (in bits) in big-endian format
	toBigEndian( totalBitsH, footer + iNeededZeros );
	toBigEndian( totalBitsL, footer + iNeededZeros + 4 );

	// finish the final block
	addData( (char*)footer, iNeededZeros + 8 );

	Bu::String sRet( 20 );

	unsigned char *digest = (unsigned char *)sRet.getStr();

	// copy the digest bytes
	toBigEndian( uH0, digest );
	toBigEndian( uH1, digest + 4 );
	toBigEndian( uH2, digest + 8 );
	toBigEndian( uH3, digest + 12 );
	toBigEndian( uH4, digest + 16 );

	// return the digest
	return sRet;
}

void Bu::Sha1::writeResult( Bu::Stream &sOut )
{
	sOut.write( getResult() );
}

void Bu::Sha1::process()
{
	int t;
	uint32_t a, b, c, d, e, K, f, W[80];

	// starting values
	a = uH0;
	b = uH1;
	c = uH2;
	d = uH3;
	e = uH4;

	// copy and expand the message block
	for( t = 0; t < 16; t++ ) W[t] = (uBytes[t*4] << 24)
									+(uBytes[t*4 + 1] << 16)
									+(uBytes[t*4 + 2] << 8)
									+ uBytes[t*4 + 3];
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
	uH0 += a;
	uH1 += b;
	uH2 += c;
	uH3 += d;
	uH4 += e;

	//printf( "Current: %08x %08x %08x %08x %08x\n",H0,H1,H2,H3,H4 );
	/* all bytes have been processed */
	iUnprocessedBytes = 0;
}

uint32_t Bu::Sha1::lrot( uint32_t x, int bits )
{
	return (x<<bits) | (x>>(32 - bits));
}

void Bu::Sha1::toBigEndian( uint32_t num, unsigned char* byte )
{
	byte[0] = (unsigned char)(num>>24);
	byte[1] = (unsigned char)(num>>16);
	byte[2] = (unsigned char)(num>>8);
	byte[3] = (unsigned char)num;
}

