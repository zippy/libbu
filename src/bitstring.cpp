#include "bitstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define random() rand()
#endif

#define bitsToBytes( nBits ) (((nBits/8)+((nBits%8)?(1):(0))));

Bu::BitString::BitString()
{
	caData = NULL;
	cTopByteMask = 0;
	nBits = nBytes = 0;
}

Bu::BitString::BitString( const Bu::BitString &xSrc )
{
	nBits = xSrc.nBits;
	nBytes = xSrc.nBytes;
	cTopByteMask = xSrc.cTopByteMask;
	caData = new unsigned char[nBytes];
	memcpy( caData, xSrc.caData, nBytes );

	fixup();
}

Bu::BitString::BitString( long nNewBits, bool bFillRandomly )
{
	long j;
	nBits = nNewBits;
	nBytes = bitsToBytes( nNewBits );//(nNewBits/8)+((nNewBits%8)?(1):(0));
	caData = new unsigned char[nBytes];

	// This can either mean that there are a multiple of eight bits or zero, if there are zero you're an idiot
	// (zero can't happen, because we would allocate an extra byte and never use it)
	if( (nBits%8 == 0) )
	{
		cTopByteMask = 0xFF;
	}
	else
	{
		cTopByteMask = 0;
		for( j = 0; j < (nBits%8); j++ )
		{
			cTopByteMask |= (1<<j);
		}
	}

	if( bFillRandomly )
	{
		// rand() only returns a value up to RAND_MAX (0x7FFF on my system) so I'll just use the low order byte)
		for( j = 0; j < nBytes; j++ )
		{
			caData[j] = (unsigned char)(random() & 0xFF);
		}
	}
	else
	{
		clearString();
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
	nBits = xSrc.nBits;
	nBytes = xSrc.nBytes;
	cTopByteMask = xSrc.cTopByteMask;
	caData = new unsigned char[nBytes];
	memcpy( caData, xSrc.caData, nBytes );

	fixup();

	return *this;
}

Bu::BitString Bu::BitString::operator~()
{
	Bu::BitString xRet( *this );

	for( int j = 0; j < xRet.nBytes; j++ )
	{
		xRet.caData[j] = ~xRet.caData[j];
	}

	xRet.fixup();

	return xRet;
}

Bu::BitString Bu::BitString::operator<<( const long nAmt )
{
	if( nAmt == 0 )
	{
		return (*this);
	}
	//int nByteShift = nAmt/8;

	Bu::BitString xSub( getBitLength() );

	long shft = (nAmt%8);
	long base = (nAmt/8);
	unsigned char lowmask=0;
	for( long j = 0; j < 8-shft; j++ )
	{
		lowmask |= (1<<j);
	}
	for( long j = 0; j < xSub.nBytes; j++ )
	{
		xSub.caData[base+j] = ((caData[j]>>shft)&(lowmask)) | ((caData[j+1]<<(8-shft))&(~lowmask));
	}
	xSub.fixup();

	return xSub;
}

Bu::BitString Bu::BitString::operator>>( const long nAmt )
{
	if( nAmt == 0 )
	{
		return (*this);
	}
	return (*this);
}

void Bu::BitString::shiftLeft( long nAmt )
{
	if( nAmt == 0 )
	{
		return;
	}
	else if( nAmt < 0 )
	{
		shiftRight( -nAmt );
		return;
	}

	long nByteShift = nAmt/8;
	long nBitShift = nAmt%8;

	long j;
	for( j = nBytes-1; j >= 0; j-- )
	{
		caData[j] = (((j-nByteShift)<0)?(0):((caData[j-nByteShift]<<nBitShift))) | (((j-nByteShift-1)<0)?(0):((caData[j-nByteShift-1]>>(8-nBitShift))));
	}

	fixup();
}

void Bu::BitString::shiftRight( long nAmt )
{
	if( nAmt == 0 )
	{
		return;
	}
	else if( nAmt < 0 )
	{
		shiftLeft( -nAmt );
		return;
	}

	long nByteShift = nAmt/8;
	long nBitShift = nAmt%8;

	long j;
	for( j = 0; j < nBytes; j++ )
	{
		caData[j] = (((j+nByteShift)>nBytes)?(0):((caData[j+nByteShift]>>nBitShift))) | (((j+nByteShift+1)>nBytes)?(0):((caData[j+nByteShift+1]<<(8-nBitShift))));
	}

	fixup();
}
/*
long Bu::BitString::bitsToBytes( long nBits )
{
	return (nBits/8)+((nBits%8)?(1):(0));
}
*/
void Bu::BitString::fixup()
{
	if( caData != NULL )
	{
		caData[nBytes-1] &= cTopByteMask;
	}
}

void Bu::BitString::setBit( long nBit, bool bBitState )
{
	if( bBitState )
	{
		caData[nBit/8] |= (1<<(nBit%8));
	}
	else
	{
		caData[nBit/8] &= ~(1<<(nBit%8));
	}
}

void Bu::BitString::flipBit( long nBit )
{
	caData[nBit/8] ^= (1<<(nBit%8));
}

bool Bu::BitString::getBit( long nBit )
{
	if( nBit >= nBits || nBit < 0 ) return false;
	if( (caData[nBit/8] & (1<<(nBit%8))) == 0 )
	{
		return false;
	}
	return true;
}

long Bu::BitString::getBitLength()
{
	return nBits;
}

class Bu::BitString Bu::BitString::getSubString( long nLower, long nUpper )
{
	if( nUpper == 0 || nUpper < nLower ) nUpper = nBits;

	Bu::BitString xSub( nUpper-nLower+1 );

	long shft = (nLower%8);
	long base = (nLower/8);
	unsigned char lowmask=0;
	for( long j = 0; j < 8-shft; j++ )
	{
		lowmask |= (1<<j);
	}
	for( long j = 0; j < xSub.nBytes; j++ )
	{
		xSub.caData[j] = ((caData[base+j]>>shft)&(lowmask)) | ((caData[base+j+1]<<(8-shft))&(~lowmask));
	}
	xSub.fixup();

	return xSub;
}

long Bu::BitString::toLong( long nStart, long nSize )
{
	if( nSize < 1 ) nSize = 1;
	if( nSize > 32 ) nSize = 32;
	if( nStart < 0 ) return 0;
	if( nStart+nSize > getBitLength() ) return 0;

	Bu::BitString tmpo;
	tmpo = getSubString( nStart, nStart+nSize-1 );
	long x = *((long *)tmpo.caData);

	return x;
}
/*
std::string Bu::BitString::toString( bool bAddSpacers )
{
	long nSz = nBits;
	if( bAddSpacers )
	{
		nSz += (nBits/8);
		if( nBits%8 == 0 ) nSz--;
	}
	std::string xStr;

	int bw=0;
	int of=0;
	for( int j = nBits-1; j >= 0; j-- )
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
			if( bw >= 8 && j < nBits-1 )
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
void Bu::BitString::clearString()
{
	if( caData != NULL )
	{
		memset( caData, 0, nBytes );
	}
}

bool Bu::BitString::setBitLength( long nLength, bool bClear )
{
	if( nBits != nLength )
	{
		if( bClear || caData == NULL )
		{
			//long j;
			nBits = nLength;
			nBytes = bitsToBytes( nLength );//(nNewBits/8)+((nNewBits%8)?(1):(0));
			if( caData != NULL ) delete[] caData;
			caData = new unsigned char[nBytes];
			memset( caData, 0, nBytes );
		}
		else
		{
			//long j;
			nBits = nLength;
			long nNewBytes = bitsToBytes( nLength );//(nNewBits/8)+((nNewBits%8)?(1):(0));
			unsigned char *tmp = caData;
			caData = new unsigned char[nBytes];
			if( nNewBytes < nBytes )
			{
				memcpy( caData, tmp, nNewBytes );
			}
			else
			{
				memcpy( caData, tmp, nBytes );
			}
			delete[] tmp;
			nBytes = nNewBytes;
		}

		// This can either mean that there are a multiple of eight bits or zero, if there are zero you're an idiot
		// (zero can't happen, because we would allocate an extra byte and never use it)
		if( (nBits%8 == 0) )
		{
			cTopByteMask = 0xFF;
		}
		else
		{
			cTopByteMask = 0;
			for( long j = 0; j < (nBits%8); j++ )
			{
				cTopByteMask |= (1<<j);
			}
		}
	}
	else if( bClear )
	{
		clearString();
	}

	return true;
}

void Bu::BitString::randomize()
{
	if( caData != NULL )
	{
		for( int j = 0; j < nBytes; j++ )
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
		for( long j = 0; j < nBytes; j++ )
		{
			caData[j] = ~caData[j];
		}
		fixup();
	}
}

long Bu::BitString::getHighestOrderBitPos()
{
	for( long j = nBits-1; j >= 0; j-- )
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
	fwrite( &nBits, sizeof(long), 1, fh );
	fwrite( caData, sizeof(char), nBytes, fh );
	
	return true;
}

bool Bu::BitString::readFromFile( FILE *fh )
{
	fread( &nBits, sizeof(long), 1, fh );
	
	nBytes = bitsToBytes( nBits );
	if( caData ) delete[] caData;
	caData = new unsigned char[nBytes];

	fread( caData, sizeof(char), nBytes, fh );

	if( (nBits%8 == 0) )
	{
		cTopByteMask = 0xFF;
	}
	else
	{
		cTopByteMask = 0;
		for( int j = 0; j < (nBits%8); j++ )
		{
			cTopByteMask |= (1<<j);
		}
	}
	
	fixup();

	return true;
}*/
