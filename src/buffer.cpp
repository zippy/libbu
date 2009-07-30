/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/buffer.h"

Bu::Buffer::Buffer( Bu::Stream &rNext, int iBufSize ) :
	Bu::Filter( rNext ),
	sSoFar( 0 ),
	iBufSize( iBufSize ),
	sReadBuf( NULL ),
	sWriteBuf( NULL ),
	iReadBufFill( 0 ),
	iReadPos( 0 ),
	iWriteBufFill( 0 ),
	iWritePos( 0 )
{
	sReadBuf = new char[iBufSize];
	sWriteBuf = new char[iBufSize];
}

Bu::Buffer::~Buffer()
{
	delete[] sReadBuf;
	delete[] sWriteBuf;
}

void Bu::Buffer::start()
{
}

size_t Bu::Buffer::stop()
{
}

void Bu::Buffer::fillReadBuf()
{
	if( iReadBufFill+iReadPos < iBufSize )
	{
		printf("Buffer: Attempting to read %db.\n", iBufSize-iReadBufFill-iReadPos );
		iReadBufFill += rNext.read(
			sReadBuf+iReadPos+iReadBufFill,
			iBufSize-iReadBufFill-iReadPos
			);
		printf("Buffer: Read from stream, %db now in buffer.\n", iReadBufFill );
	}
}

size_t Bu::Buffer::read( void *pBuf, size_t nBytes )
{
	if( nBytes <= 0 )
	{
		fillReadBuf();
		return 0;
	}

	size_t nTotRead = 0;
//	fillReadBuf();

	do
	{
		int iAmnt = nBytes-nTotRead;
		if( iAmnt > iReadBufFill )
		{
			iAmnt = iReadBufFill;
		}
		if( iAmnt > 0 )
		{
			memcpy( ((char *)pBuf)+nTotRead, sReadBuf+iReadPos, iAmnt );
			iReadPos += iAmnt;
			nTotRead += iAmnt;
			iReadBufFill -= iAmnt;
		}
		if( iReadBufFill == 0 )
		{
			iReadPos = 0;
			fillReadBuf();
		}
	}
	while( nTotRead < nBytes && iReadBufFill > 0 );

	printf("Buffer:  %db returned, %db remain in buffer.\n", nTotRead, iReadBufFill );

	return nTotRead;
}

size_t Bu::Buffer::write( const void *pBuf, size_t nBytes )
{
	size_t nTotWrote = 0;

	do
	{
		int iAmnt = nBytes-nTotWrote;
		if( iAmnt > iBufSize-iWritePos-iWriteBufFill )
		{
			iAmnt = iBufSize-iWritePos-iWriteBufFill;
		}
		if( iAmnt > 0 )
		{
			memcpy(
				sWriteBuf+iWritePos+iWriteBufFill,
				((char *)pBuf)+nTotWrote,
				iAmnt
				);
			nTotWrote += iAmnt;
			iWriteBufFill += iAmnt;
			printf("Buffer: Moved %db to write buffer, %db filled now.\n",
					iAmnt, iWriteBufFill );
		}
		while( iWritePos+iWriteBufFill == iBufSize )
		{
			printf("iWritePos = %d\n", iWritePos );
			int iWr = rNext.write( sWriteBuf+iWritePos, iWriteBufFill );
			printf("Buffer: Wrote %db from buffer to stream, %db filled now.\n", iWr, iWriteBufFill-iWr );
			if( iWr == 0 )
			{
				return nTotWrote;
			}
			else if( iWr == iWriteBufFill )
			{
				iWritePos = iWriteBufFill = 0;
			}
			else
			{
				iWritePos += iWr;
				iWriteBufFill -= iWr;
			}
		}
	}
	while( nTotWrote < nBytes && iWriteBufFill < iBufSize+iWritePos );

	return nTotWrote;
}

void Bu::Buffer::flush()
{
	if( iWriteBufFill > 0 )
	{
		printf("Buffer: Flushing remaining data, %db.\n", iWriteBufFill );
		int iWr = 0;
		do
		{
			iWr = rNext.write( sWriteBuf+iWritePos, iWriteBufFill );
			printf("Buffer: %db written to stream.\n", iWr );
			iWritePos += iWr;
			iWriteBufFill -= iWr;
		} while( iWriteBufFill > 0 && iWr > 0 );
	}
}

