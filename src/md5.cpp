/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bu/md5.h"
#include "bu/stream.h"

#ifdef SYSTEM_BIG_ENDIAN
# define toLittleEndian( a, b ) _toLittleEndian( a, b )
#else
# define toLittleEndian( a, b ) (void)0
#endif

Bu::Md5::Md5()
{
	reset();
}

Bu::Md5::~Md5()
{
}

void Bu::Md5::reset()
{
	// These are the magic seed numbers...

	sum[0] = 0x67452301U;
	sum[1] = 0xEFCDAB89U;
	sum[2] = 0x98BADCFEU;
	sum[3] = 0x10325476U;

	uBits[0] = 0;
	uBits[1] = 0;
}

void Bu::Md5::setSalt( const Bu::String & /*sSalt*/ )
{
}

void Bu::Md5::addData( const void *sVData, int iSize )
{
	const char *sData = (const char *)sVData;
	uint32_t t;

	t = uBits[0];
	if( (uBits[0] = t + ((uint32_t)iSize << 3)) < t )
		uBits[1]++;
	uBits[1] += iSize >> 29;

	t = (t >> 3) & 0x3f;	/* How many bytes we have buffered */

	/* Handle any leading odd-sized chunks */
	if( t )
	{
		unsigned char *p = (unsigned char *) inbuf + t;

		t = 64 - t;
		if( iSize < t ) {
			memcpy( p, sData, iSize );
			return;
		}
		memcpy( p, sData, t );
		toLittleEndian( inbuf, 16 );
		compBlock( sum, (uint32_t *)inbuf );
		sData += t;
		iSize -= t;
	}

	/* Process data in 64-byte chunks */
	while( iSize >= 64 )
	{
		memcpy( inbuf, sData, 64 );
		toLittleEndian( inbuf, 16 );
		compBlock( sum, (uint32_t *)inbuf );
		sData += 64;
		iSize -= 64;
	}

	/* Handle any remaining bytes of data. */
	memcpy( inbuf, sData, iSize );
}

Bu::String Bu::Md5::getResult()
{
	uint32_t lsum[4];
	compCap( lsum );
	return Bu::String( (const char *)lsum, 4*4 );
}

void Bu::Md5::writeResult( Bu::Stream &sOut )
{
	uint32_t lsum[4];
	compCap( lsum );
	sOut.write( lsum, 4*4 );
}

void Bu::Md5::compCap( uint32_t *sumout )
{
	uint8_t tmpbuf[64];
	memcpy( sumout, sum, 4*4 );
	memcpy( tmpbuf, inbuf, 64 );

	uint32_t count;
	uint8_t *p;

	/* Compute number of bytes mod 64 */
	count = (uBits[0] >> 3) & 0x3F;

	/* Set the first char of padding to 0x80.  This is safe since there is
	   always at least one byte free */
	p = tmpbuf + count;
	*p++ = 0x80;

	/* Bytes of padding needed to make 64 bytes */
	count = 64 - 1 - count;

	/* Pad out to 56 mod 64 */
	if (count < 8) {
		/* Two lots of padding:  Pad the first block to 64 bytes */
		memset( p, 0, count );
		toLittleEndian( tmpbuf, 16 );
		compBlock( sumout, (uint32_t *)tmpbuf );

		/* Now fill the next block with 56 bytes */
		memset( tmpbuf, 0, 56);
	} else {
		/* Pad block to 56 bytes */
		memset( p, 0, count - 8);
	}
	toLittleEndian( tmpbuf, 14 );

	/* Append length in bits and transform */
	((uint32_t *) tmpbuf)[14] = uBits[0];
	((uint32_t *) tmpbuf)[15] = uBits[1];

	compBlock( sumout, (uint32_t *)tmpbuf );
	toLittleEndian((unsigned char *)sumout, 4);
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

void Bu::Md5::compBlock( uint32_t *lsum, uint32_t *x )
{
	register uint32_t a, b, c, d;
	a = lsum[0];
	b = lsum[1];
	c = lsum[2];
	d = lsum[3];

    MD5STEP(F1, a, b, c, d, x[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, x[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, x[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, x[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, x[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, x[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, x[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, x[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, x[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, x[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, x[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, x[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, x[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, x[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, x[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, x[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, x[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, x[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, x[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, x[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, x[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, x[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, x[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, x[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, x[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, x[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, x[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, x[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, x[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, x[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, x[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, x[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, x[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, x[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, x[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, x[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, x[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, x[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, x[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, x[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, x[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, x[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, x[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, x[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, x[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, x[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, x[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, x[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, x[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, x[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, x[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, x[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, x[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, x[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, x[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, x[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, x[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, x[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, x[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, x[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, x[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, x[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, x[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, x[9] + 0xeb86d391, 21);

	lsum[0] += a;
	lsum[1] += b;
	lsum[2] += c;
	lsum[3] += d;
}

void Bu::Md5::_toLittleEndian( uint8_t *buf, uint32_t count )
{
	uint32_t t;
	do {
		t = (uint32_t) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
			((unsigned) buf[1] << 8 | buf[0]);
		*(uint32_t *) buf = t;
		buf += 4;
	} while( --count );
}
