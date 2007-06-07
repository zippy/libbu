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
	printf("-> Bu::BZip2::~BZip2()\n");	
	stop();
}

void Bu::BZip2::start()
{
	printf("-> Bu::BZip2::start()\n");	
	printf("Hey, it's starting...\n");
	bzState.state = NULL;
	bzState.bzalloc = NULL;
	bzState.bzfree = NULL;
	bzState.opaque = NULL;

	nBufSize = 50000;
	pBuf = new char[nBufSize];
}

void Bu::BZip2::stop()
{
	printf("-> Bu::BZip2::stop()\n");
	if( bzState.state )
	{
		if( bReading )
		{
		}
		else
		{
			for(;;)
			{
				bzState.next_in = NULL;
				bzState.avail_in = 0;
				bzState.avail_out = nBufSize;
				bzState.next_out = pBuf;
				int res = BZ2_bzCompress( &bzState, BZ_FINISH );
				if( bzState.avail_out < nBufSize )
				{
					rNext.write( pBuf, nBufSize-bzState.avail_out );
				}
				if( res == BZ_STREAM_END )
					break;
			}
			BZ2_bzCompressEnd( &bzState );
		}
	}
}

void Bu::BZip2::bzError( int code )
{
	switch( code )
	{
		case BZ_OK:
			return;

		case BZ_CONFIG_ERROR:
			throw ExceptionBase("The bzip2 library has been miscompiled.");

		case BZ_PARAM_ERROR:
			throw ExceptionBase("bzip2 parameter error.");

		case BZ_MEM_ERROR:
			throw ExceptionBase("Not enough memory available for bzip2.");
	}
}

size_t Bu::BZip2::read( void *pData, size_t nBytes )
{
	if( !bzState.state )
	{
		bReading = true;
	}
	if( bReading == false )
		throw ExceptionBase("This bzip2 filter is in writing mode, you can't read.");
	//bzState.next_in = pData;
	//bzState.avail_in = nSizeIn;
	
	//printf("%db at [%08X] (%db)\n", bzState.avail_in, (uint32_t)bzState.next_in, bzState.total_in_lo32 );
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

	bzState.next_in = (char *)pData;
	bzState.avail_in = nBytes;
	for(;;)
	{
		bzState.avail_out = nBufSize;
		bzState.next_out = pBuf;

		BZ2_bzCompress( &bzState, BZ_RUN );

		if( bzState.avail_out < nBufSize )
		{
			rNext.write( pBuf, nBufSize-bzState.avail_out );
		}
		if( bzState.avail_in == 0 )
			break;
	}

	return 0;
}

