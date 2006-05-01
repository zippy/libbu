#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

// This is a fun macro that tells us where the length char goes after the data
// section in the padded data segment.  It's short for OBfuscation LOCaction.
#define OBLOC(len)	((((len + 64) >> 9) << 4) + 14)
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

md5::md5()
{
}

md5::~md5()
{
}

/*
 * Calculate the MD5 of an array of little-endian words, and a bit length
 */
void md5::core_md5( long *x, long len, md5sum *output )
{
	long a =  1732584193, olda;
	long b = -271733879, oldb;
	long c = -1732584194, oldc;
	long d =  271733878, oldd;

	for( long i = 0; i < len; i += 16 )
	{
		olda = a;
		oldb = b;
		oldc = c;
		oldd = d;
 
		a = md5_ff(a, b, c, d, x[i+ 0], 7 , -680876936);
		d = md5_ff(d, a, b, c, x[i+ 1], 12, -389564586);
		c = md5_ff(c, d, a, b, x[i+ 2], 17,  606105819);
		b = md5_ff(b, c, d, a, x[i+ 3], 22, -1044525330);
		a = md5_ff(a, b, c, d, x[i+ 4], 7 , -176418897);
		d = md5_ff(d, a, b, c, x[i+ 5], 12,  1200080426);
		c = md5_ff(c, d, a, b, x[i+ 6], 17, -1473231341);
		b = md5_ff(b, c, d, a, x[i+ 7], 22, -45705983);
		a = md5_ff(a, b, c, d, x[i+ 8], 7 ,  1770035416);
		d = md5_ff(d, a, b, c, x[i+ 9], 12, -1958414417);
		c = md5_ff(c, d, a, b, x[i+10], 17, -42063);
		b = md5_ff(b, c, d, a, x[i+11], 22, -1990404162);
		a = md5_ff(a, b, c, d, x[i+12], 7 ,  1804603682);
		d = md5_ff(d, a, b, c, x[i+13], 12, -40341101);
		c = md5_ff(c, d, a, b, x[i+14], 17, -1502002290);
		b = md5_ff(b, c, d, a, x[i+15], 22,  1236535329);

		a = md5_gg(a, b, c, d, x[i+ 1], 5 , -165796510);
		d = md5_gg(d, a, b, c, x[i+ 6], 9 , -1069501632);
		c = md5_gg(c, d, a, b, x[i+11], 14,  643717713);
		b = md5_gg(b, c, d, a, x[i+ 0], 20, -373897302);
		a = md5_gg(a, b, c, d, x[i+ 5], 5 , -701558691);
		d = md5_gg(d, a, b, c, x[i+10], 9 ,  38016083);
		c = md5_gg(c, d, a, b, x[i+15], 14, -660478335);
		b = md5_gg(b, c, d, a, x[i+ 4], 20, -405537848);
		a = md5_gg(a, b, c, d, x[i+ 9], 5 ,  568446438);
		d = md5_gg(d, a, b, c, x[i+14], 9 , -1019803690);
		c = md5_gg(c, d, a, b, x[i+ 3], 14, -187363961);
		b = md5_gg(b, c, d, a, x[i+ 8], 20,  1163531501);
		a = md5_gg(a, b, c, d, x[i+13], 5 , -1444681467);
		d = md5_gg(d, a, b, c, x[i+ 2], 9 , -51403784);
		c = md5_gg(c, d, a, b, x[i+ 7], 14,  1735328473);
		b = md5_gg(b, c, d, a, x[i+12], 20, -1926607734);

		a = md5_hh(a, b, c, d, x[i+ 5], 4 , -378558);
		d = md5_hh(d, a, b, c, x[i+ 8], 11, -2022574463);
		c = md5_hh(c, d, a, b, x[i+11], 16,  1839030562);
		b = md5_hh(b, c, d, a, x[i+14], 23, -35309556);
		a = md5_hh(a, b, c, d, x[i+ 1], 4 , -1530992060);
		d = md5_hh(d, a, b, c, x[i+ 4], 11,  1272893353);
		c = md5_hh(c, d, a, b, x[i+ 7], 16, -155497632);
		b = md5_hh(b, c, d, a, x[i+10], 23, -1094730640);
		a = md5_hh(a, b, c, d, x[i+13], 4 ,  681279174);
		d = md5_hh(d, a, b, c, x[i+ 0], 11, -358537222);
		c = md5_hh(c, d, a, b, x[i+ 3], 16, -722521979);
		b = md5_hh(b, c, d, a, x[i+ 6], 23,  76029189);
		a = md5_hh(a, b, c, d, x[i+ 9], 4 , -640364487);
		d = md5_hh(d, a, b, c, x[i+12], 11, -421815835);
		c = md5_hh(c, d, a, b, x[i+15], 16,  530742520);
		b = md5_hh(b, c, d, a, x[i+ 2], 23, -995338651);

		a = md5_ii(a, b, c, d, x[i+ 0], 6 , -198630844);
		d = md5_ii(d, a, b, c, x[i+ 7], 10,  1126891415);
		c = md5_ii(c, d, a, b, x[i+14], 15, -1416354905);
		b = md5_ii(b, c, d, a, x[i+ 5], 21, -57434055);
		a = md5_ii(a, b, c, d, x[i+12], 6 ,  1700485571);
		d = md5_ii(d, a, b, c, x[i+ 3], 10, -1894986606);
		c = md5_ii(c, d, a, b, x[i+10], 15, -1051523);
		b = md5_ii(b, c, d, a, x[i+ 1], 21, -2054922799);
		a = md5_ii(a, b, c, d, x[i+ 8], 6 ,  1873313359);
		d = md5_ii(d, a, b, c, x[i+15], 10, -30611744);
		c = md5_ii(c, d, a, b, x[i+ 6], 15, -1560198380);
		b = md5_ii(b, c, d, a, x[i+13], 21,  1309151649);
		a = md5_ii(a, b, c, d, x[i+ 4], 6 , -145523070);
		d = md5_ii(d, a, b, c, x[i+11], 10, -1120210379);
		c = md5_ii(c, d, a, b, x[i+ 2], 15,  718787259);
		b = md5_ii(b, c, d, a, x[i+ 9], 21, -343485551);

		a = a + olda;
		b = b + oldb;
		c = c + oldc;
		d = d + oldd;
	}
  
	output->data[0] = a;
	output->data[1] = b;
	output->data[2] = c;
	output->data[3] = d;
	delete[] x;
} 

long *md5::c2l( const char *str, long len, long *nNewLen )
{
	long len8 = len*8;
	long mlen = OBLOC( len8 );
    long flen = (((mlen/16)+((mlen%16)?(1):(0))))*16;
	long *aBin = new long[flen];
	memset( aBin, 0, flen );
	
	for( long i = 0; i < len8; i+=8 )
	{
		aBin[i>>5] |= ((long)str[i/8]) << (i%32);
	}

	aBin[len8 >> 5] |= 0x80 << ((len8) % 32);
	aBin[OBLOC( len8 )] = len8;

	(*nNewLen) = flen;

	return aBin;
}

void md5::l2hexstr( long *binarray, char *str )
{
	static const char hex_tab[] = {"0123456789abcdef"};
	//static char str[33];
	
	int k = 0;
	for( int i = 0; i < 16; i++)
	{
		str[k++] = hex_tab[(binarray[i>>2] >> ((i%4)*8+4)) & 0xF];
		str[k++] = hex_tab[(binarray[i>>2] >> ((i%4)*8  )) & 0xF];
	}
}

void md5::sumString( md5sum *pSum, const char *sStr )
{
	sumData( pSum, sStr, strlen( sStr ) );
}

void md5::sumData( md5sum *pSum, const char *aData, long nLen )
{
	long nNewLen;
	long *aOb = c2l( aData, nLen, &nNewLen );
	core_md5( aOb, nNewLen, pSum );
}

void md5::sumToHex( md5sum *pSum, char *sHex )
{
	l2hexstr( pSum->data, sHex );
}

