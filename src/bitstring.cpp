/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/bitstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bu/exceptionbase.h"

#ifdef _WIN32
#define random() rand()
#endif

#define bitsToBytes( iBits ) (((iBits/8)+((iBits%8)?(1):(0))));

Bu::BitString::BitString()
{
	caData = NULL;
	cTopByteMask = 0;
	iBits = iBytes = 0;
}

Bu::BitString::BitString( const Bu::BitString &xSrc )
{
	iBits = xSrc.iBits;
	iBytes = xSrc.iBytes;
	cTopByteMask = xSrc.cTopByteMask;
	caData = new unsigned char[iBytes];
	memcpy( caData, xSrc.caData, iBytes );

	fixup();
}

Bu::BitString::BitString( long iNewBits, bool bFillRandomly )
{
	long j;
	iBits = iNewBits;
	iBytes = bitsToBytes( iNewBits );//(iNewBits/8)+((iNewBits%8)?(1):(0));
	caData = new unsigned char[iBytes];

	setMask();

	if( bFillRandomly )
	{
		// rand() only returns a value up to RAND_MAX (0x7FFF on my system) so
		// I'll just use the low order byte)
		for( j = 0; j < iBytes; j++ )
		{
			caData[j] = (unsigned char)(random() & 0xFF);
		}
	}
	else
	{
		clear();
	}

	fixup();
}

Bu::BitString::~BitString()
{
	if( caData != NULL ) delete[] caData;
}

Bu::BitString &Bu::BitString::operator=( const Bu::BitString &xSrc )
{
	if( caData != NULL )
	{
		delete[] caData;
	}
	iBits = xSrc.iBits;
	iBytes = xSrc.iBytes;
	cTopByteMask = xSrc.cTopByteMask;
	caData = new unsigned char[iBytes];
	memcpy( caData, xSrc.caData, iBytes );

	fixup();

	return *this;
}

Bu::BitString Bu::BitString::operator~()
{
	Bu::BitString xRet( *this );

	for( int j = 0; j < xRet.iBytes; j++ )
	{
		xRet.caData[j] = ~xRet.caData[j];
	}

	xRet.fixup();

	return xRet;
}

Bu::BitString Bu::BitString::operator<<( const long iAmt )
{
	if( iAmt == 0 )
	{
		return (*this);
	}
	//int iByteShift = iAmt/8;

	Bu::BitString xSub( getSize() );

	long shft = (iAmt%8);
	long base = (iAmt/8);
	unsigned char lowmask=0;
	for( long j = 0; j < 8-shft; j++ )
	{
		lowmask |= (1<<j);
	}
	for( long j = 0; j < xSub.iBytes; j++ )
	{
		xSub.caData[base+j] = ((caData[j]>>shft)&(lowmask)) | ((caData[j+1]<<(8-shft))&(~lowmask));
	}
	xSub.fixup();

	return xSub;
}

Bu::BitString Bu::BitString::operator>>( const long iAmt )
{
	if( iAmt == 0 )
	{
		return (*this);
	}
	return (*this);
}

void Bu::BitString::shiftLeft( long iAmt )
{
	if( iAmt == 0 )
	{
		return;
	}
	else if( iAmt < 0 )
	{
		shiftRight( -iAmt );
		return;
	}

	long iByteShift = iAmt/8;
	long iBitShift = iAmt%8;

	long j;
	for( j = iBytes-1; j >= 0; j-- )
	{
		caData[j] = (((j-iByteShift)<0)?(0):((caData[j-iByteShift]<<iBitShift))) | (((j-iByteShift-1)<0)?(0):((caData[j-iByteShift-1]>>(8-iBitShift))));
	}

	fixup();
}

void Bu::BitString::shiftRight( long iAmt )
{
	if( iAmt == 0 )
	{
		return;
	}
	else if( iAmt < 0 )
	{
		shiftLeft( -iAmt );
		return;
	}

	long iByteShift = iAmt/8;
	long iBitShift = iAmt%8;

	long j;
	for( j = 0; j < iBytes; j++ )
	{
		caData[j] = (((j+iByteShift)>iBytes)?(0):((caData[j+iByteShift]>>iBitShift))) | (((j+iByteShift+1)>iBytes)?(0):((caData[j+iByteShift+1]<<(8-iBitShift))));
	}

	fixup();
}
/*
long Bu::BitString::bitsToBytes( long iBits )
{
	return (iBits/8)+((iBits%8)?(1):(0));
}
*/
void Bu::BitString::fixup()
{
	if( caData != NULL )
	{
		caData[iBytes-1] &= cTopByteMask;
	}
}

void Bu::BitString::setBit( long iBit, bool bBitState )
{
	if( iBit < 0 || iBit >= iBits )
		throw Bu::ExceptionBase("bit out of range: %d in (0-%d)", iBit, iBits );
	if( bBitState )
	{
		caData[iBit/8] |= (1<<(iBit%8));
	}
	else
	{
		caData[iBit/8] &= ~(1<<(iBit%8));
	}
}

void Bu::BitString::flipBit( long iBit )
{
	caData[iBit/8] ^= (1<<(iBit%8));
}

bool Bu::BitString::getBit( long iBit )
{
	if( iBit >= iBits || iBit < 0 ) return false;
	if( (caData[iBit/8] & (1<<(iBit%8))) == 0 )
	{
		return false;
	}
	return true;
}

long Bu::BitString::getBitLength()
{
	return iBits;
}

long Bu::BitString::getSize()
{
	return iBits;
}

class Bu::BitString Bu::BitString::getSubString( long iLower, long iUpper )
{
	if( iUpper == 0 || iUpper < iLower ) iUpper = iBits;

	Bu::BitString xSub( iUpper-iLower+1 );

	long shft = (iLower%8);
	long base = (iLower/8);
	unsigned char lowmask=0;
	for( long j = 0; j < 8-shft; j++ )
	{
		lowmask |= (1<<j);
	}
	for( long j = 0; j < xSub.iBytes; j++ )
	{
		xSub.caData[j] = ((caData[base+j]>>shft)&(lowmask)) | ((caData[base+j+1]<<(8-shft))&(~lowmask));
	}
	xSub.fixup();

	return xSub;
}

long Bu::BitString::toLong( long iStart, long iSize )
{
	if( iSize < 1 ) iSize = 1;
	if( iSize > 32 ) iSize = 32;
	if( iStart < 0 ) return 0;
	if( iStart+iSize > getSize() ) return 0;

	Bu::BitString tmpo;
	tmpo = getSubString( iStart, iStart+iSize-1 );
	long x = *((long *)tmpo.caData);

	return x;
}
/*
std::string Bu::BitString::toString( bool bAddSpacers )
{
	long iSz = iBits;
	if( bAddSpacers )
	{
		iSz += (iBits/8);
		if( iBits%8 == 0 ) iSz--;
	}
	std::string xStr;

	int bw=0;
	int of=0;
	for( int j = iBits-1; j >= 0; j-- )
	{
		if( getBit( j ) )
		{
			xStr += '1';
		}
		else
		{
			xStr += '0';
		}

		if( bAddSpacers )
		{
			bw++;
			if( bw >= 8 && j < iBits-1 )
			{
				bw = 0;
				of++;
				xStr += ' ';
			}
		}
	}

	return xStr;
}
*/
void Bu::BitString::clear()
{
	if( caData != NULL )
	{
		memset( caData, 0, iBytes );
	}
}

bool Bu::BitString::setBitLength( long iLength, bool bClear )
{
	return setSize( iLength, bClear );
}

bool Bu::BitString::setSize( long iLength, bool bClear )
{
	// First, if there's nothing, then allocate an empty one.
	if( caData == NULL )
	{
		iBits = iLength;
		iBytes = bitsToBytes( iLength );
		caData = new unsigned char[iBytes];
		memset( caData, 0, iBytes );
		return true;
	}

	// If the new length is the same as the old, don't do anything, but do
	// check to see if we should still clear the data.
	if( iBits != iLength )
	{
		// Ok, we are changing the number if bits, but are we changing the
		// number of bytes?
		long iNewBytes = bitsToBytes( iLength );
		if( iBytes == iNewBytes )
		{
			// No? That makes life easier
			iBits = iLength;
			setMask();
			if( bClear )
			{
				clear();
			}
		}
		else
		{
			// Ok, reallocate and copy...
			iBits = iLength;
//			long iNewBytes = bitsToBytes( iLength );
			if( bClear )
			{
				delete[] caData;
				caData = new unsigned char[iNewBytes];
				memset( caData, 0, iNewBytes );
			}
			else
			{
				unsigned char *tmp = caData;
				caData = new unsigned char[iNewBytes];
				if( iNewBytes < iBytes )
				{
					memcpy( caData, tmp, iNewBytes );
				}
				else
				{
					memcpy( caData, tmp, iBytes );
				}
				delete[] tmp;
			}
			iBytes = iNewBytes;
			
			setMask();
		}

	}
	else if( bClear )
	{
		clear();
	}

	return true;
}

void Bu::BitString::setMask()
{
	// This can either mean that there are a multiple of eight bits or
	// zero, if there are zero you're an idiot (zero can't happen, because
	// we would allocate an extra byte and never use it)
	if( (iBits%8 == 0) )
	{
		cTopByteMask = 0xFF;
	}
	else
	{
		cTopByteMask = 0;
		for( long j = 0; j < (iBits%8); j++ )
		{
			cTopByteMask |= (1<<j);
		}
	}
}

void Bu::BitString::randomize()
{
	if( caData != NULL )
	{
		for( int j = 0; j < iBytes; j++ )
		{
			caData[j] = (unsigned char)(random() & 0xFF);
		}
		fixup();
	}
}

void Bu::BitString::invert()
{
	if( caData != NULL )
	{
		for( long j = 0; j < iBytes; j++ )
		{
			caData[j] = ~caData[j];
		}
		fixup();
	}
}

long Bu::BitString::getHighestOrderBitPos()
{
	for( long j = iBits-1; j >= 0; j-- )
	{
		if( getBit( j ) )
		{
			return j;
		}
	}

	return -1;
}
/*
bool Bu::BitString::writeToFile( FILE *fh )
{
	fwrite( &iBits, sizeof(long), 1, fh );
	fwrite( caData, sizeof(char), iBytes, fh );
	
	return true;
}

bool Bu::BitString::readFromFile( FILE *fh )
{
	fread( &iBits, sizeof(long), 1, fh );
	
	iBytes = bitsToBytes( iBits );
	if( caData ) delete[] caData;
	caData = new unsigned char[iBytes];

	fread( caData, sizeof(char), iBytes, fh );

	setMask();
	
	fixup();

	return true;
}*/
