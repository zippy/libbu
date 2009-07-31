/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/protocol.h"
#include "bu/multiserver.h"
#include "bu/client.h"
#include "bu/filter.h"

using namespace Bu;

class Rot13Filter : public Filter
{
public:
	Rot13Filter( Stream &next ) :
		Filter( next )
	{
	}

	virtual ~Rot13Filter()
	{
	}

	virtual void start()
	{
	}

	virtual size_t stop()
	{
		return 0;
	}

	virtual size_t read( void *pBuf, size_t nBytes )
	{
		return rNext.read( pBuf, nBytes );
	}
	
	virtual size_t write( const void *pBuf, size_t nBytes )
	{
		const char *cBuf = (const char *)pBuf;
		char *buf = new char[nBytes];
		for( size_t j = 0; j < nBytes; j++ )
		{
			if( cBuf[j] >= 'a' && cBuf[j] <= 'z' )
				buf[j] = (cBuf[j]-'a'+13)%26+'a';
			else if( cBuf[j] >= 'A' && cBuf[j] <= 'Z' )
				buf[j] = (cBuf[j]-'A'+13)%26+'A';
			else
				buf[j] = cBuf[j];
		}
		rNext.write( buf, nBytes );
		delete[] buf;
		return nBytes;
	}
};

class Rot13Protocol : public Protocol
{
public:
	void onNewConnection( Bu::Client *pClient )
	{
		pClient->pushFilter<Rot13Filter>();
	}

	void onNewData( Bu::Client *pClient )
	{
		pClient->write( pClient->getInput().getStr(), pClient->getInputSize() );
		pClient->seek( pClient->getInputSize() );
	}
};

int main()
{
	MultiServer xSrv;

	xSrv.setTimeout( 5 );
	xSrv.addProtocol( genProtocol<Rot13Protocol>, 9999 );

	for(;;)
		xSrv.scan();

	return 0;
}

