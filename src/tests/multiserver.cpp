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
		pClient->write( pClient->getInput() );
		pClient->seek( pClient->getInputSize() );
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
		Bu::FString sTmp = pClient->getInput();
		for( int j = 0; j < sTmp.getSize(); j++ )
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
		pClient->write( sTmp );
		pClient->seek( pClient->getInputSize() );
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

