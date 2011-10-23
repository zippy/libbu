/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/deflate.h"
#include "bu/trace.h"

using namespace Bu;

Bu::Deflate::Deflate( Bu::Stream &rNext, int nCompression, Format eFmt ) :
	Bu::Filter( rNext ),
	nCompression( nCompression ),
	sTotalOut( 0 ),
	eFmt( eFmt ),
	bEos( false )
{
	TRACE( nCompression );
	start();
}

Bu::Deflate::~Deflate()
{
	TRACE();
	stop();
}

void Bu::Deflate::start()
{
	TRACE();
	zState.zalloc = NULL;
	zState.zfree = NULL;
	zState.opaque = NULL;
	zState.state = NULL;

	nBufSize = 64*1024;
	pBuf = new char[nBufSize];
}

Bu::size Bu::Deflate::stop()
{
	TRACE();
	if( zState.state )
	{
		if( bReading )
		{
			inflateEnd( &zState );
			delete[] pBuf;
			pBuf = NULL;
			return 0;
		}
		else
		{
			for(;;)
			{
				zState.next_in = NULL;
				zState.avail_in = 0;
				zState.avail_out = nBufSize;
				zState.next_out = (Bytef *)pBuf;
				int res = deflate( &zState, Z_FINISH );
				if( zState.avail_out < nBufSize )
				{
					sTotalOut += rNext.write( pBuf, nBufSize-zState.avail_out );
				}
				if( res == Z_STREAM_END )
					break;
			}
			deflateEnd( &zState );
			delete[] pBuf;
			pBuf = NULL;
			return sTotalOut;
		}
	}
	return 0;
}

void Bu::Deflate::zError( int code )
{
	TRACE( code );
	switch( code )
	{
		case Z_OK:
		case Z_STREAM_END:
		case Z_NEED_DICT:
			return;

		case Z_ERRNO:
			throw ExceptionBase("Deflate: Errno - %s", zState.msg );

		case Z_STREAM_ERROR:
			throw ExceptionBase("Deflate: Stream Error - %s", zState.msg );

		case Z_DATA_ERROR:
			throw ExceptionBase("Deflate: Data Error - %s", zState.msg );

		case Z_MEM_ERROR:
			throw ExceptionBase("Deflate: Mem Error - %s", zState.msg );

		case Z_BUF_ERROR:
			throw ExceptionBase("Deflate: Buf Error - %s", zState.msg );

		case Z_VERSION_ERROR:
			throw ExceptionBase("Deflate: Version Error - %s", zState.msg );

		default:
			throw ExceptionBase("Deflate: Unknown error encountered - %s.", zState.msg );
	
	}
}

Bu::size Bu::Deflate::read( void *pData, Bu::size nBytes )
{
	TRACE( pData, nBytes );
	if( !zState.state )
	{
		bReading = true;
		if( eFmt&AutoDetect )
			inflateInit2( &zState, 32+15 ); // Auto-detect, large window
		else if( eFmt == Raw )
			inflateInit2( &zState, -15 ); // Raw
		else if( eFmt == Zlib )
			inflateInit2( &zState, 15 ); // Zlib
		else if( eFmt == Gzip )
			inflateInit2( &zState, 16+15 ); // GZip
		else
			throw Bu::ExceptionBase("Format mode for deflate read.");
		zState.next_in = (Bytef *)pBuf;
		zState.avail_in = 0;
	}
	if( bReading == false )
		throw ExceptionBase("This deflate filter is in writing mode, you can't read.");

	int nRead = 0;
	int nReadTotal = zState.total_out;
	zState.next_out = (Bytef *)pData;
	zState.avail_out = nBytes;
	for(;;)
	{
		int ret = inflate( &zState, Z_NO_FLUSH );
		printf("inflate returned %d; avail in=%d, out=%d\n", ret,
				zState.avail_in, zState.avail_out );
	
		nReadTotal += nRead-zState.avail_out;

		if( ret == Z_STREAM_END )
		{
			bEos = true;
			if( zState.avail_in > 0 )
			{
				if( rNext.isSeekable() )
				{
					rNext.seek( -zState.avail_in );
				}
			}
			return nBytes-zState.avail_out;
		}
		if( ret != Z_BUF_ERROR )
			zError( ret );

		if( zState.avail_out )
		{
			if( zState.avail_in == 0 )
			{
				nRead = rNext.read( pBuf, nBufSize );
				if( nRead == 0 && rNext.isEos() )
				{
					throw Bu::ExceptionBase("Premature end of underlying "
							"stream found reading deflate stream.");
				}
				zState.next_in = (Bytef *)pBuf;
				zState.avail_in = nRead;
			}
		}
		else
		{
			return nBytes-zState.avail_out;
		}
	}
	return 0;
}

Bu::size Bu::Deflate::write( const void *pData, Bu::size nBytes )
{
	TRACE( pData, nBytes );
	if( !zState.state )
	{
		bReading = false;
		int iFmt = eFmt&Gzip;
		if( iFmt == Raw )
			deflateInit2( &zState, nCompression, Z_DEFLATED, -15, 9,
				Z_DEFAULT_STRATEGY );
		else if( iFmt == Zlib )
			deflateInit2( &zState, nCompression, Z_DEFLATED, 15, 9,
				Z_DEFAULT_STRATEGY );
		else if( iFmt == Gzip )
			deflateInit2( &zState, nCompression, Z_DEFLATED, 16+15, 9,
				Z_DEFAULT_STRATEGY );
		else
			throw Bu::ExceptionBase("Invalid format for deflate.");
	}
	if( bReading == true )
		throw ExceptionBase("This deflate filter is in reading mode, you can't write.");

	zState.next_in = (Bytef *)pData;
	zState.avail_in = nBytes;
	for(;;)
	{
		zState.avail_out = nBufSize;
		zState.next_out = (Bytef *)pBuf;

		zError( deflate( &zState, Z_NO_FLUSH ) );

		if( zState.avail_out < nBufSize )
		{
			sTotalOut += rNext.write( pBuf, nBufSize-zState.avail_out );
		}
		if( zState.avail_in == 0 )
			break;
	}

	return nBytes;
}

bool Bu::Deflate::isOpen()
{
	TRACE();
	return (zState.state != NULL);
}

bool Bu::Deflate::isEos()
{
	TRACE();
	return bEos;
}

Bu::size Bu::Deflate::getCompressedSize()
{
	return sTotalOut;
}

