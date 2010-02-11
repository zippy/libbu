#include "bu/newline.h"

Bu::NewLine::NewLine( Bu::Stream &rNext ) :
	Bu::Filter( rNext ),
	bExChar( false )
{
}

Bu::NewLine::~NewLine()
{
}

void Bu::NewLine::start()
{
}

size_t Bu::NewLine::stop()
{
	return 0;
}

size_t Bu::NewLine::read( void *pBuf, size_t iAmnt )
{
	size_t iRead = rNext.read( pBuf, iAmnt );
	size_t iOffset = 0;

	for( size_t i = 0; i < iRead; i++ )
	{
		if( pBuf[i] == '\r' )
		{
			pBuf[i+iOffset] = '\n';
			if( pBuf[j+1] == '\n' )
			{
				iOffset--;
			}
		}
		else if( pBuf[i] == '\n' )
		{
			if( pBuf[j+1] == '\r' )
			{
				iOffset--;
			}
		}
		else if( iOffset )
		{
			pBuf[i+iOffset] = pBuf[i];
		}
	}

	iRead += iOffset;
}

size_t Bu::NewLine::write( const void *pBuf, size_t iAmnt )
{
}

