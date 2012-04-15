/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/buffer.h"

Bu::Buffer::Buffer( Bu::Stream &rNext, int iWhat, int iBufSize ) :
	Bu::Filter( rNext ),
	sSoFar( 0 ),
	iBufSize( iBufSize ),
	sReadBuf( NULL ),
	sWriteBuf( NULL ),
	iReadBufFill( 0 ),
	iReadPos( 0 ),
	iWriteBufFill( 0 ),
	iWritePos( 0 ),
	iWhat( iWhat )
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

Bu::size Bu::Buffer::stop()
{
	iReadBufFill = iReadPos = iWriteBufFill = iWritePos = 0;
	return sSoFar;
}

void Bu::Buffer::fillReadBuf()
{
	if( iReadBufFill+iReadPos < iBufSize )
	{
		iReadBufFill += rNext.read(
			sReadBuf+iReadPos+iReadBufFill,
			iBufSize-iReadBufFill-iReadPos
			);
	}
}

Bu::size Bu::Buffer::read( void *pBuf, Bu::size nBytes )
{
	if( (iWhat&Read) == 0 )
		return rNext.read( pBuf, nBytes );

	if( nBytes <= 0 )
	{
		fillReadBuf();
		return 0;
	}

	Bu::size nTotRead = 0;
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

	//printf("Buffer:  %db returned, %db remain in buffer.\n", nTotRead, iReadBufFill );

	return nTotRead;
}

Bu::size Bu::Buffer::write( const void *pBuf, Bu::size nBytes )
{
	if( (iWhat&Write) == 0 )
		return rNext.write( pBuf, nBytes );

	Bu::size nTotWrote = 0;

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
			//printf("Buffer: Moved %db to write buffer, %db filled now.\n",
					//iAmnt, iWriteBufFill );
		}
		while( iWritePos+iWriteBufFill == iBufSize )
		{
			//printf("iWritePos = %d\n", iWritePos );
			int iWr = rNext.write( sWriteBuf+iWritePos, iWriteBufFill );
			//printf("Buffer: Wrote %db from buffer to stream, %db filled now.\n", iWr, iWriteBufFill-iWr );
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
	if( (iWhat&Write) == 0 )
		return rNext.flush();

	if( iWriteBufFill > 0 )
	{
		//printf("Buffer: Flushing remaining data, %db.\n", iWriteBufFill );
		int iWr = 0;
		do
		{
			iWr = rNext.write( sWriteBuf+iWritePos, iWriteBufFill );
			//printf("Buffer: %db written to stream.\n", iWr );
			iWritePos += iWr;
			iWriteBufFill -= iWr;
		} while( iWriteBufFill > 0 && iWr > 0 );
	}
}

bool Bu::Buffer::isEos()
{
	return (iReadPos >= (iReadBufFill-1)) && (rNext.isEos());
}

