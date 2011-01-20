/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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

	virtual Bu::size stop()
	{
		return 0;
	}

	virtual Bu::size read( void *pBuf, Bu::size nBytes )
	{
		return rNext.read( pBuf, nBytes );
	}
	
	virtual Bu::size write( const void *pBuf, Bu::size nBytes )
	{
		const char *cBuf = (const char *)pBuf;
		char *buf = new char[nBytes];
		for( Bu::size j = 0; j < nBytes; j++ )
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
		char buf[1024];
		while( pClient->hasInput() )
		{
			int iAmnt = pClient->read( buf, 1024 );
			pClient->write( buf, iAmnt );
		}
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

