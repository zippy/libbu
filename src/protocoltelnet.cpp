#include "bu/protocoltelnet.h"
#include "bu/client.h"

#define CODE_SE		'\xf0'	/**< End of subnegotiation params. */
#define CODE_NOP	'\xf1'	/**< No operation (keep-alive). */
#define CODE_DM		'\xf2'	/**< Datastream side of a Synch. */
#define CODE_BRK	'\xf3'	/**< Break character. */
#define CODE_IP		'\xf4' /**< Interrupt Process character. */
#define CODE_AO		'\xf5' /**< Abort Output character. */
#define CODE_AYT	'\xf6' /**< Are You There? character. */
#define CODE_EC		'\xf7' /**< Erase Character character. */
#define CODE_EL		'\xf8'	/**< Erase Line character. */
#define CODE_GA		'\xf9' /**< Go Ahead signal. */
#define CODE_SB		'\xfa'	/**< Begin subnegotiation options. */
#define CODE_WILL	'\xfb'	/**< Desire to do something. */
#define CODE_WONT	'\xfc'	/**< Refuse to perform. */
#define CODE_DO		'\xfd'	/**< Request option. */
#define CODE_DONT	'\xfe'	/**< Demand a stop. */

#define CODE_IAC	'\xff'	/**< Interpret-As-Command. */

#define OPT_BINARY	'\x00'	/**< Binary mode (file transfers?). */
#define OPT_ECHO	'\x01'	/**< (local) Echo mode. */

Bu::ProtocolTelnet::ProtocolTelnet() :
	oBinary(	*this, OPT_BINARY ),
	oEcho(		*this, OPT_ECHO )
{
}

Bu::ProtocolTelnet::~ProtocolTelnet()
{
}

void Bu::ProtocolTelnet::onNewConnection( Bu::Client *pClient )
{
}

void Bu::ProtocolTelnet::onNewData( Bu::Client *pClient )
{
}



Bu::ProtocolTelnet::Option::Option( Bu::ProtocolTelnet &rPT, char cCode ) :
	rPT( rPT ),
	fOpts( 0 ),
	cCode( cCode )
{
	rPT.hOpts.insert( cCode, this );
}

Bu::ProtocolTelnet::Option::~Option()
{
}

void Bu::ProtocolTelnet::Option::localEnable( bool bSet )
{
	if( bSet == (bool)(!(fOpts&fLocalCant)) ) return;

	if( bSet )
		fOpts &= ~fLocalCant;
	else
		fOpts |= fLocalCant;
}

void Bu::ProtocolTelnet::Option::localSet( bool bSet )
{
	if( bSet == (bool)(fOpts&fLocalIs) ) return;

	char buf[2];

	if( bSet )
	{
		buf[0] = CODE_WILL;
		buf[1] = cCode;
		rPT.pClient->write( buf, 2 );
	}
	else
	{
		buf[0] = CODE_WONT;
		buf[1] = cCode;
		rPT.pClient->write( buf, 2 );
	}
}

bool Bu::ProtocolTelnet::Option::isLocalEnabled()
{
	return (bool)(!(fOpts&fLocalCant));
}

bool Bu::ProtocolTelnet::Option::isLocalSet()
{
	return (bool)(fOpts&fLocalIs);
}

void Bu::ProtocolTelnet::Option::remoteEnable( bool bSet )
{
	return;
}

void Bu::ProtocolTelnet::Option::remoteSet( bool bSet )
{
	if( bSet == (bool)(fOpts&fRemoteIs) ) return;

	char buf[2];

	if( bSet )
	{
		buf[0] = CODE_DO;
		buf[1] = cCode;
		rPT.pClient->write( buf, 2 );
	}
	else
	{
		buf[0] = CODE_DONT;
		buf[1] = cCode;
		rPT.pClient->write( buf, 2 );
	}
}

bool Bu::ProtocolTelnet::Option::isRemoteEnabled()
{
	return (bool)(!(fOpts&fRemoteCant));
}

bool Bu::ProtocolTelnet::Option::isRemoteSet()
{
	return (bool)(fOpts&fRemoteIs);
}

