/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bu/md5.h"
#include "bu/stream.h"


// This performs a wrapping bitwise shift, kinda' fun!

#define bit_roll( num, cnt ) \
	(((num) << (cnt)) | (((num) >> (32 - (cnt))) & ~(-1<<(cnt))))

//#define md5_cmn( q, a, b, x, s, t ) (bit_roll((a + q + x + t), s) + b)

// The following are handy wrappers for the cmn function
#define md5_ff( a, b, c, d, x, s, t ) \
	(md5_cmn((b & c) | ((~b) & d), a, b, x, s, t))

#define md5_gg( a, b, c, d, x, s, t ) \
	(md5_cmn((b & d) | (c & (~d)), a, b, x, s, t))

#define md5_hh( a, b, c, d, x, s, t ) \
	(md5_cmn(b ^ c ^ d, a, b, x, s, t))

#define md5_ii( a, b, c, d, x, s, t ) \
	(md5_cmn(c ^ (b | (~d)), a, b, x, s, t))

inline long md5_cmn( long q, long a, long b, long x, long s, long t )
{
	return bit_roll((a + q + x + t), s) + b;
}

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

	sum[0] = 1732584193;
	sum[1] = -271733879;
	sum[2] = -1732584194;
	sum[3] =  271733878;

	iBytes = 0;
	memset( inbuf, 0, 4*16 );
	iFill = 0;
}

void Bu::Md5::setSalt( const Bu::FString & /*sSalt*/ )
{
}

void Bu::Md5::addData( const void *sVData, int iSize )
{
	const char *sData = (const char *)sVData;

	int iInPos = 0;
	for(;;)
	{
		for( ; iFill < 16*4 && iInPos < iSize; iFill++, iInPos++ )
		{
			inbuf[iFill>>2] |= ((long)sData[iInPos]) << ((iFill*8)%32);
		}
		if( iFill < 16*4 )
			break;
		compBlock( inbuf, sum );
		memset( inbuf, 0, 4*16 );
		iFill = 0;
	}
	iBytes += iSize;
}

Bu::FString Bu::Md5::getResult()
{
	long lsum[4];
	compCap( lsum );
	return Bu::FString( (const char *)lsum, 4*4 );
}

void Bu::Md5::writeResult( Bu::Stream &sOut )
{
	long lsum[4];
	compCap( lsum );
	sOut.write( lsum, 4*4 );
}

void Bu::Md5::compCap( long *sumout )
{
	memcpy( sumout, sum, 4*4 );
	long lbuf[16];
	memcpy( lbuf, inbuf, 4*16 );

	lbuf[iFill>>2] |= 0x80 << ((iFill*8)%32);
	uint64_t iBits = iBytes*8;
	if( iBytes > 0 && iFill>>2 >= 14 )
	{
		compBlock( lbuf, sumout );
		memset( lbuf, 0, 4*16 );
		memcpy( lbuf+14, &iBits, 8 );
		compBlock( lbuf, sumout );
	}
	else
	{
		memcpy( lbuf+14, &iBits, 8 );
		compBlock( lbuf, sumout );
	}
}

void Bu::Md5::compBlock( long *x, long *lsum )
{
	long a = lsum[0];
	long b = lsum[1];
	long c = lsum[2];
	long d = lsum[3];

	a = md5_ff(a, b, c, d, x[ 0], 7 , -680876936);
	d = md5_ff(d, a, b, c, x[ 1], 12, -389564586);
	c = md5_ff(c, d, a, b, x[ 2], 17,  606105819);
	b = md5_ff(b, c, d, a, x[ 3], 22, -1044525330);
	a = md5_ff(a, b, c, d, x[ 4], 7 , -176418897);
	d = md5_ff(d, a, b, c, x[ 5], 12,  1200080426);
	c = md5_ff(c, d, a, b, x[ 6], 17, -1473231341);
	b = md5_ff(b, c, d, a, x[ 7], 22, -45705983);
	a = md5_ff(a, b, c, d, x[ 8], 7 ,  1770035416);
	d = md5_ff(d, a, b, c, x[ 9], 12, -1958414417);
	c = md5_ff(c, d, a, b, x[10], 17, -42063);
	b = md5_ff(b, c, d, a, x[11], 22, -1990404162);
	a = md5_ff(a, b, c, d, x[12], 7 ,  1804603682);
	d = md5_ff(d, a, b, c, x[13], 12, -40341101);
	c = md5_ff(c, d, a, b, x[14], 17, -1502002290);
	b = md5_ff(b, c, d, a, x[15], 22,  1236535329);

	a = md5_gg(a, b, c, d, x[ 1], 5 , -165796510);
	d = md5_gg(d, a, b, c, x[ 6], 9 , -1069501632);
	c = md5_gg(c, d, a, b, x[11], 14,  643717713);
	b = md5_gg(b, c, d, a, x[ 0], 20, -373897302);
	a = md5_gg(a, b, c, d, x[ 5], 5 , -701558691);
	d = md5_gg(d, a, b, c, x[10], 9 ,  38016083);
	c = md5_gg(c, d, a, b, x[15], 14, -660478335);
	b = md5_gg(b, c, d, a, x[ 4], 20, -405537848);
	a = md5_gg(a, b, c, d, x[ 9], 5 ,  568446438);
	d = md5_gg(d, a, b, c, x[14], 9 , -1019803690);
	c = md5_gg(c, d, a, b, x[ 3], 14, -187363961);
	b = md5_gg(b, c, d, a, x[ 8], 20,  1163531501);
	a = md5_gg(a, b, c, d, x[13], 5 , -1444681467);
	d = md5_gg(d, a, b, c, x[ 2], 9 , -51403784);
	c = md5_gg(c, d, a, b, x[ 7], 14,  1735328473);
	b = md5_gg(b, c, d, a, x[12], 20, -1926607734);

	a = md5_hh(a, b, c, d, x[ 5], 4 , -378558);
	d = md5_hh(d, a, b, c, x[ 8], 11, -2022574463);
	c = md5_hh(c, d, a, b, x[11], 16,  1839030562);
	b = md5_hh(b, c, d, a, x[14], 23, -35309556);
	a = md5_hh(a, b, c, d, x[ 1], 4 , -1530992060);
	d = md5_hh(d, a, b, c, x[ 4], 11,  1272893353);
	c = md5_hh(c, d, a, b, x[ 7], 16, -155497632);
	b = md5_hh(b, c, d, a, x[10], 23, -1094730640);
	a = md5_hh(a, b, c, d, x[13], 4 ,  681279174);
	d = md5_hh(d, a, b, c, x[ 0], 11, -358537222);
	c = md5_hh(c, d, a, b, x[ 3], 16, -722521979);
	b = md5_hh(b, c, d, a, x[ 6], 23,  76029189);
	a = md5_hh(a, b, c, d, x[ 9], 4 , -640364487);
	d = md5_hh(d, a, b, c, x[12], 11, -421815835);
	c = md5_hh(c, d, a, b, x[15], 16,  530742520);
	b = md5_hh(b, c, d, a, x[ 2], 23, -995338651);

	a = md5_ii(a, b, c, d, x[ 0], 6 , -198630844);
	d = md5_ii(d, a, b, c, x[ 7], 10,  1126891415);
	c = md5_ii(c, d, a, b, x[14], 15, -1416354905);
	b = md5_ii(b, c, d, a, x[ 5], 21, -57434055);
	a = md5_ii(a, b, c, d, x[12], 6 ,  1700485571);
	d = md5_ii(d, a, b, c, x[ 3], 10, -1894986606);
	c = md5_ii(c, d, a, b, x[10], 15, -1051523);
	b = md5_ii(b, c, d, a, x[ 1], 21, -2054922799);
	a = md5_ii(a, b, c, d, x[ 8], 6 ,  1873313359);
	d = md5_ii(d, a, b, c, x[15], 10, -30611744);
	c = md5_ii(c, d, a, b, x[ 6], 15, -1560198380);
	b = md5_ii(b, c, d, a, x[13], 21,  1309151649);
	a = md5_ii(a, b, c, d, x[ 4], 6 , -145523070);
	d = md5_ii(d, a, b, c, x[11], 10, -1120210379);
	c = md5_ii(c, d, a, b, x[ 2], 15,  718787259);
	b = md5_ii(b, c, d, a, x[ 9], 21, -343485551);

	lsum[0] = a + lsum[0];
	lsum[1] = b + lsum[1];
	lsum[2] = c + lsum[2];
	lsum[3] = d + lsum[3];
}

