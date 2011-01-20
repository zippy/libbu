/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/multiserver.h"
#include "bu/protocol.h"
#include "bu/client.h"

class ProtocolRaw : public Bu::Protocol
{
public:
	virtual void onNewConnection( Bu::Client *pClient )
	{
		pClient->write("Raw echo\n");
	}

	virtual void onNewData( Bu::Client *pClient )
	{
		char buf[1024];
		while( pClient->hasInput() )
		{
			int iAmnt = pClient->read( buf, 1024 );
			pClient->write( buf, iAmnt );
		}
	}
};

class ProtocolRot13 : public Bu::Protocol
{
public:
	virtual void onNewConnection( Bu::Client *pClient )
	{
		pClient->write("Rot13 echo\n");
	}

	virtual void onNewData( Bu::Client *pClient )
	{
		while( pClient->hasInput() )
		{
			char sTmp[1024];
			int iAmnt = pClient->read( sTmp, 1024 );
			for( int j = 0; j < iAmnt; j++ )
			{
				if( sTmp[j] >= 'a' && sTmp[j] <= 'z' )
				{
					sTmp[j] = ((sTmp[j]-'a'+13)%26) + 'a';
				}
				else if( sTmp[j] >= 'A' && sTmp[j] <= 'Z' )
				{
					sTmp[j] = ((sTmp[j]-'A'+13)%26) + 'A';
				}
			}
			pClient->write( sTmp, iAmnt );
		}
	}
};

int main()
{
	Bu::MultiServer msMain;

	msMain.addProtocol( Bu::genProtocol<ProtocolRaw>, 5550 );
	msMain.addProtocol( Bu::genProtocol<ProtocolRot13>, 5551 );
	msMain.setTimeout( 5, 0 );

	for(;;)
	{
		msMain.scan();
	}

	return 0;
}

