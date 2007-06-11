#include "bu/bzip2.h"
#include "bu/exceptions.h"

using namespace Bu;

Bu::BZip2::BZip2( Bu::Stream &rNext, int nCompression ) :
	Bu::Filter( rNext ),
	nCompression( nCompression )
{
	start();
}

Bu::BZip2::~BZip2()
{
	stop();
}

void Bu::BZip2::start()
{
	bzState.state = NULL;
	bzState.bzalloc = NULL;
	bzState.bzfree = NULL;
	bzState.opaque = NULL;

	nBufSize = 50000;
	pBuf = new char[nBufSize];
}

size_t Bu::BZip2::stop()
{
	if( bzState.state )
	{
		if( bReading )
		{
			BZ2_bzDecompressEnd( &bzState );
			return 0;
		}
		else
		{
			size_t sTotal = 0;
			for(;;)
			{
				bzState.next_in = NULL;
				bzState.avail_in = 0;
				bzState.avail_out = nBufSize;
				bzState.next_out = pBuf;
				int res = BZ2_bzCompress( &bzState, BZ_FINISH );
				if( bzState.avail_out < nBufSize )
				{
					sTotal += rNext.write( pBuf, nBufSize-bzState.avail_out );
				}
				if( res == BZ_STREAM_END )
					break;
			}
			BZ2_bzCompressEnd( &bzState );
			return sTotal;
		}
	}
	return 0;
}

void Bu::BZip2::bzError( int code )
{
	switch( code )
	{
		case BZ_OK:
			printf("<BZ_OK>\n"); return;
		case BZ_RUN_OK:
			printf("<BZ_RUN_OK>\n"); return;
		case BZ_FLUSH_OK:
			printf("<BZ_FLUSH_OK>\n"); return;
		case BZ_FINISH_OK:
			printf("<BZ_FINISH_OK>\n"); return;
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

size_t Bu::BZip2::read( void *pData, size_t nBytes )
{
	if( !bzState.state )
	{
		bReading = true;
		BZ2_bzDecompressInit( &bzState, 0, 0 );
		bzState.next_in = pBuf;
		bzState.avail_in = 0;
	}
	if( bReading == false )
		throw ExceptionBase("This bzip2 filter is in writing mode, you can't read.");

	int nRead = 0;
	int nReadTotal = bzState.total_out_lo32;
	for(;;)
	{
		bzState.next_out = (char *)pData;
		bzState.avail_out = nBytes;
		printf(" (pre) in: %db, out: %db\n", bzState.avail_in, bzState.avail_out );
		int ret = BZ2_bzDecompress( &bzState );
		printf("(post) in: %db, out: %db\n", bzState.avail_in, bzState.avail_out );
	
		nReadTotal += nRead-bzState.avail_out;

		if( ret == BZ_STREAM_END )
		{
			printf("<BZ_STREAM_END>\n");
			if( bzState.avail_in > 0 )
			{
				if( rNext.canSeek() )
				{
					rNext.seek( -bzState.avail_in );
				}
			}
			return nBytes-bzState.avail_out;
		}
		bzError( ret );

		if( bzState.avail_out )
		{
			printf("Still more to fill, in: %db, out: %db\n", bzState.avail_in, bzState.avail_out );

			if( bzState.avail_in == 0 )
			{
				nRead = rNext.read( pBuf, nBufSize );
				bzState.next_in = pBuf;
				bzState.avail_in = nRead;
			}
		}
		else
		{
			return nBytes-bzState.avail_out;
		}
	}
	return 0;
}

size_t Bu::BZip2::write( const void *pData, size_t nBytes )
{
	if( !bzState.state )
	{
		bReading = false;
		BZ2_bzCompressInit( &bzState, nCompression, 0, 30 );
	}
	if( bReading == true )
		throw ExceptionBase("This bzip2 filter is in reading mode, you can't write.");

	size_t sTotalOut = 0;
	bzState.next_in = (char *)pData;
	bzState.avail_in = nBytes;
	for(;;)
	{
		bzState.avail_out = nBufSize;
		bzState.next_out = pBuf;

		bzError( BZ2_bzCompress( &bzState, BZ_RUN ) );

		if( bzState.avail_out < nBufSize )
		{
			sTotalOut += rNext.write( pBuf, nBufSize-bzState.avail_out );
		}
		if( bzState.avail_in == 0 )
			break;
	}

	return sTotalOut;
}

