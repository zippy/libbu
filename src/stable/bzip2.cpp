/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/bzip2.h"
#include "bu/trace.h"

#include <bzlib.h>

#define pState ((bz_stream *)prState)

using namespace Bu;

Bu::BZip2::BZip2( Bu::Stream &rNext, int nCompression ) :
	Bu::Filter( rNext ),
	prState( NULL ),
	nCompression( nCompression ),
	sTotalOut( 0 )
{
	TRACE( nCompression );
	start();
}

Bu::BZip2::~BZip2()
{
	TRACE();
	stop();
}

void Bu::BZip2::start()
{
	TRACE();

	prState = new bz_stream;
	pState->state = NULL;
	pState->bzalloc = NULL;
	pState->bzfree = NULL;
	pState->opaque = NULL;

	nBufSize = 64*1024;
	pBuf = new char[nBufSize];
}

Bu::size Bu::BZip2::stop()
{
	TRACE();
	if( pState->state )
	{
		if( bReading )
		{
			BZ2_bzDecompressEnd( pState );
			delete[] pBuf;
			pBuf = NULL;
			delete pState;
			prState = NULL;
			return 0;
		}
		else
		{
//			Bu::size sTotal = 0;
			for(;;)
			{
				pState->next_in = NULL;
				pState->avail_in = 0;
				pState->avail_out = nBufSize;
				pState->next_out = pBuf;
				int res = BZ2_bzCompress( pState, BZ_FINISH );
				if( pState->avail_out < nBufSize )
				{
					sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
				}
				if( res == BZ_STREAM_END )
					break;
			}
			BZ2_bzCompressEnd( pState );
			delete[] pBuf;
			pBuf = NULL;
			delete pState;
			prState = NULL;
			return sTotalOut;
		}
	}
	return 0;
}

void Bu::BZip2::bzError( int code )
{
	TRACE( code );
	switch( code )
	{
		case BZ_OK:
		case BZ_RUN_OK:
		case BZ_FLUSH_OK:
		case BZ_FINISH_OK:
			return;

		case BZ_CONFIG_ERROR:
			throw ExceptionBase("BZip2: Library configured improperly, reinstall.");

		case BZ_SEQUENCE_ERROR:
			throw ExceptionBase("BZip2: Functions were called in an invalid sequence.");

		case BZ_PARAM_ERROR:
			throw ExceptionBase("BZip2: Invalid parameter was passed into a function.");

		case BZ_MEM_ERROR:
			throw ExceptionBase("BZip2: Couldn't allocate sufficient memory.");

		case BZ_DATA_ERROR:
			throw ExceptionBase("BZip2: Data was corrupted before decompression.");

		case BZ_DATA_ERROR_MAGIC:
			throw ExceptionBase("BZip2: Stream does not appear to be bzip2 data.");

		case BZ_IO_ERROR:
			throw ExceptionBase("BZip2: File couldn't be read from / written to.");

		case BZ_UNEXPECTED_EOF:
			throw ExceptionBase("BZip2: End of file encountered before end of stream.");

		case BZ_OUTBUFF_FULL:
			throw ExceptionBase("BZip2: Buffer not large enough to accomidate data.");

		default:
			throw ExceptionBase("BZip2: Unknown error encountered.");
	
	}
}

Bu::size Bu::BZip2::read( void *pData, Bu::size nBytes )
{
	TRACE( pData, nBytes );
	if( !pState->state )
	{
		bReading = true;
		BZ2_bzDecompressInit( pState, 0, 0 );
		pState->next_in = pBuf;
		pState->avail_in = 0;
	}
	if( bReading == false )
		throw ExceptionBase("This bzip2 filter is in writing mode, you can't read.");

	int nRead = 0;
	int nReadTotal = pState->total_out_lo32;
	pState->next_out = (char *)pData;
	pState->avail_out = nBytes;
	for(;;)
	{
		int ret = BZ2_bzDecompress( pState );
	
		nReadTotal += nRead-pState->avail_out;

		if( ret == BZ_STREAM_END )
		{
			if( pState->avail_in > 0 )
			{
				if( rNext.isSeekable() )
				{
					rNext.seek( -pState->avail_in );
				}
			}
			return nBytes-pState->avail_out;
		}
		bzError( ret );

		if( pState->avail_out )
		{
			if( pState->avail_in == 0 )
			{
				nRead = rNext.read( pBuf, nBufSize );
				if( nRead == 0 && rNext.isEos() )
				{
					throw Bu::ExceptionBase("Premature end of underlying "
							"stream found reading bzip2 stream.");
				}
				pState->next_in = pBuf;
				pState->avail_in = nRead;
			}
		}
		else
		{
			return nBytes-pState->avail_out;
		}
	}
	return 0;
}

Bu::size Bu::BZip2::write( const void *pData, Bu::size nBytes )
{
	TRACE( pData, nBytes );
	if( !pState->state )
	{
		bReading = false;
		BZ2_bzCompressInit( pState, nCompression, 0, 30 );
	}
	if( bReading == true )
		throw ExceptionBase("This bzip2 filter is in reading mode, you can't write.");

//	Bu::size sTotalOut = 0;
	pState->next_in = (char *)pData;
	pState->avail_in = nBytes;
	for(;;)
	{
		pState->avail_out = nBufSize;
		pState->next_out = pBuf;

		bzError( BZ2_bzCompress( pState, BZ_RUN ) );

		if( pState->avail_out < nBufSize )
		{
			sTotalOut += rNext.write( pBuf, nBufSize-pState->avail_out );
		}
		if( pState->avail_in == 0 )
			break;
	}

	return nBytes;
}

bool Bu::BZip2::isOpen()
{
	TRACE();
	return (pState->state != NULL);
}

Bu::size Bu::BZip2::getCompressedSize()
{
	return sTotalOut;
}

