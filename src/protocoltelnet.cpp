/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/protocoltelnet.h"
#include "bu/client.h"

/* We apparently at least want defs for the lower 13, not sure we care about
 * the rest of the chars, maybe escape.
 */
#define CH_NUL		'\x00'	/* NUL */
#define CH_SOH		'\x01'	/* Start Of Heading */
#define CH_STX		'\x02'	/* Start of Text */
#define CH_ETX		'\x03'	/* End of Text */
#define CH_EOT		'\x04'	/* End of transmission */
#define CH_ENQ		'\x05'	/* Enquiery */
#define CH_ACK		'\x06'	/* Acknowledge */
#define CH_BEL		'\x07'	/* Bell */
#define CH_BS		'\x08'	/* Backspace */
#define CH_TAB		'\x09'	/* Horizontal Tab */
#define CH_LF		'\x0A'	/* NL Line feed, new line */
#define CH_VT		'\x0B'	/* Vertical Tab */
#define CH_FF		'\x0C'	/* Form feed, new page */
#define CH_CR		'\x0D'	/* Carriage return */
#define CH_ESC		'\x1B'	/* Escape */
#define CH_DEL		'\x7F'	/* Delete */

#define CODE_SE		'\xf0'	/* End of subnegotiation params. */
#define CODE_NOP	'\xf1'	/* No operation (keep-alive). */
#define CODE_DM		'\xf2'	/* Datastream side of a Synch. */
#define CODE_BRK	'\xf3'	/* Break character. */
#define CODE_IP		'\xf4'	/* Interrupt Process character. */
#define CODE_AO		'\xf5'	/* Abort Output character. */
#define CODE_AYT	'\xf6'	/* Are You There? character. */
#define CODE_EC		'\xf7'	/* Erase Character character. */
#define CODE_EL		'\xf8'	/* Erase Line character. */
#define CODE_GA		'\xf9'	/* Go Ahead signal. */
#define CODE_SB		'\xfa'	/* Begin subnegotiation options. */
#define CODE_WILL	'\xfb'	/* Desire to do something. */
#define CODE_WONT	'\xfc'	/* Refuse to perform. */
#define CODE_DO		'\xfd'	/* Request option. */
#define CODE_DONT	'\xfe'	/* Demand a stop. */

#define CODE_IAC	'\xff'	/* Interpret-As-Command. */

#define OPT_BINARY	'\x00'	/* Binary mode (file transfers?). */
#define OPT_ECHO	'\x01'	/* (local) Echo mode. */
#define OPT_SUPGA	'\x03'	/* Suppress Go Ahead signals. */
#define OPT_STATUS	'\x05'	/* Allow status messages. */
#define OPT_TIMING	'\x06'	/* Place a timing mark in the code. */
#define OPT_EXASCII	'\x11'	/* Extended ASCII. */
#define OPT_LOGOUT	'\x12'	/* Logout. */
#define OPT_TTYPE	'\x18'	/* Terminal Type. */
#define OPT_NAWS	'\x1f'	/* Negotiate about window size. */
#define OPT_TSPEED	'\x20'	/* Terminal Speed. */
#define OPT_NEWENV	'\x27'	/* New Environment Option. */
#define OPT_EXOPL	'\xff'	/* Can we, will we, handle extended options. */

#ifndef __TELNET_DEBUG
# define printCode( a ) (void)0
# define printOpt( a ) (void)0
#endif

Bu::ProtocolTelnet::ProtocolTelnet() :
	oBinary(	*this, OPT_BINARY ),
	oEcho(		*this, OPT_ECHO ),
	oNAWS(		*this, OPT_NAWS ),
	oSuppressGA(*this, OPT_SUPGA ),
	bCanonical( true ),
	bSubOpt( false )
{
}

Bu::ProtocolTelnet::~ProtocolTelnet()
{
}

void Bu::ProtocolTelnet::onNewConnection( Bu::Client *pClient )
{
	this->pClient = pClient;
}

void Bu::ProtocolTelnet::onNewData( Bu::Client *pClient )
{
	char bc;
	int iLeft;
	while( (iLeft = pClient->getInputSize()) )
	{
		if( bSubOpt )
		{
			pClient->peek( &bc, 1 );
			if( bc == CODE_IAC )
			{
				if( iLeft <= 1 ) return;
				char bc2;
				printCode( CODE_IAC );
				pClient->peek( &bc2, 1, 1 );
				printCode( bc2 );
				if( bc2 == CODE_SE )
				{
					bSubOpt = false;
					onSubOpt();
				}
				else if( bc2 == CODE_IAC )
				{
					sSubBuf += CODE_IAC;
				}
				else
				{
					// Error of some sort.
				}
				pClient->seek( 1 );
			}
			else
			{
				sSubBuf += bc;
			}
			pClient->seek( 1 );
		}
		else
		{
			pClient->peek( &bc, 1 );
			if( bc == CODE_IAC )
			{
				if( iLeft <= 1 ) return;
				char bc2;
				pClient->peek( &bc2, 1, 1 );
				printCode( bc );
				printCode( bc2 );

				switch( bc2 )
				{
					case CODE_WILL:
						if( iLeft <= 2 ) return;
						{
							char bc3;
							pClient->peek( &bc3, 1, 2 );
							pClient->seek( 1 );
							printOpt( bc3 );
							onWill( bc3 );
						}
						break;

					case CODE_WONT:
						if( iLeft <= 2 ) return;
						{
							char bc3;
							pClient->peek( &bc3, 1, 2 );
							pClient->seek( 1 );
							printOpt( bc3 );
							onWont( bc3 );
						}
						break;

					case CODE_DO:
						if( iLeft <= 2 ) return;
						{
							char bc3;
							pClient->peek( &bc3, 1, 2 );
							pClient->seek( 1 );
							printOpt( bc3 );
							onDo( bc3 );
						}
						break;

					case CODE_DONT:
						if( iLeft <= 2 ) return;
						{
							char bc3;
							pClient->peek( &bc3, 1, 2 );
							pClient->seek( 1 );
							printOpt( bc3 );
							onDont( bc3 );
						}
						break;

					case CODE_SB:
						if( iLeft <= 2 ) return;
						{
							pClient->peek( &cSubOpt, 1, 2 );
							pClient->seek( 1 );
							printOpt( cSubOpt );
							bSubOpt = true;
						}
						break;

					case CODE_IAC:
						sDataBuf += CODE_IAC;
						printCode( CODE_IAC );
						break;
				}
				pClient->seek( 1 );
#ifdef __TELNET_DEBUG
				printf("\n");
#endif
			}
			else if( bc == CODE_SB )
			{
			}
			else
			{
				// This is where control code handling goes
				// Also, possibly, character code conversion, although I'm not
				// sure that really matters anymore, go ASCII/UTF-8
				if( bCanonical )
				{
					if( bc < 0x20 || bc >= CH_DEL )
					{
						if( bc == CH_CR )
						{
							if( iLeft <= 1 ) return;
							char bc2;
							pClient->peek( &bc2, 1, 1 );
							if( bc2 == CH_NUL || bc2 == CH_LF )
							{
								onCtlChar( bc );
								gotLine( sDataBuf );
								sDataBuf.clear();
							}
							pClient->seek( 1 );
						}
						else
						{
							onCtlChar( bc );
						}
					}
					else
					{
						sDataBuf += bc;
						if( oEcho.isLocalSet() )
						{
							pClient->write( &bc, 1 );
#ifdef __TELNET_DEBUG
							printf("%c", bc );
							fflush( stdout );
#endif
						}
					}
				}
				else
				{
					sDataBuf += bc;
					if( oEcho.isLocalSet() )
					{
						pClient->write( &bc, 1 );
					}
				}
			}
			pClient->seek( 1 );
		}
	}

	// It's true, this code will not be executed if we only have half of an
	// IAC code or multibyte escape sequence or something, but then again, it
	// shouldn't be called then, and really, shouldn't be, it'll be called soon
	// enough, when we get the rest of that code.
	if( !bCanonical )
	{
		gotData( sDataBuf );
	}
}

void Bu::ProtocolTelnet::setCanonical( bool bCon )
{
	bCanonical = bCon;
}

bool Bu::ProtocolTelnet::isCanonical()
{
	return bCanonical;
}

void Bu::ProtocolTelnet::write( const Bu::String &sData )
{
	write( sData.getStr(), sData.getSize() );
}

void Bu::ProtocolTelnet::write( const char *pData, int iSize )
{
	int iLast = 0, j;
	for( j = iLast; j < iSize; j++ )
	{
		if( pData[j] == '\n' )
		{
			if( j+1 >= iSize ||
				(pData[j+1] != '\r' && pData[j+1] != '\0') )
			{
				pClient->write( pData+iLast, j-iLast );
				pClient->write( "\n\r", 2 );
				iLast = j+1;
			}
			else
			{
				j++;
			}
		}
	}
	if( j > iLast )
	{
		pClient->write( pData+iLast, iSize-iLast );
	}
	//pClient->write( pData, iSize );
}

void Bu::ProtocolTelnet::write( char cData )
{
	write( &cData, 1 );
}

void Bu::ProtocolTelnet::onWill( char cCode )
{
	try
	{
		Option *pOpt = hOpts[cCode];
		if( pOpt->isRemoteEnabled() )
		{
			pOpt->fOpts |= Option::fRemoteIs;
			char buf[3] = { CODE_IAC, CODE_DO, cCode };
			pClient->write( buf, 3 );
		}
		else
		{
			char buf[3] = { CODE_IAC, CODE_DONT, cCode };
			pClient->write( buf, 3 );
		}

	}
	catch( Bu::HashException &e )
	{
		char buf[3] = { CODE_IAC, CODE_DONT, cCode };
		pClient->write( buf, 3 );
	}
}

void Bu::ProtocolTelnet::onWont( char cCode )
{
	try
	{
		Option *pOpt = hOpts[cCode];

		pOpt->fOpts &= ~Option::fRemoteIs;
		char buf[3] = { CODE_IAC, CODE_DONT, cCode };
		pClient->write( buf, 3 );
	}
	catch( Bu::HashException &e )
	{
		char buf[3] = { CODE_IAC, CODE_DONT, cCode };
		pClient->write( buf, 3 );
	}
}

void Bu::ProtocolTelnet::onDo( char cCode )
{
	try
	{
		Option *pOpt = hOpts[cCode];
		if( pOpt->isLocalEnabled() )
		{
			pOpt->fOpts |= Option::fLocalIs;
			char buf[3] = { CODE_IAC, CODE_WILL, cCode };
			pClient->write( buf, 3 );
		}
		else
		{
			char buf[3] = { CODE_IAC, CODE_WONT, cCode };
			pClient->write( buf, 3 );
		}

	}
	catch( Bu::HashException &e )
	{
		char buf[3] = { CODE_IAC, CODE_WONT, cCode };
		pClient->write( buf, 3 );
	}
}

void Bu::ProtocolTelnet::onDont( char cCode )
{
	try
	{
		Option *pOpt = hOpts[cCode];

		pOpt->fOpts &= ~Option::fLocalIs;
		char buf[3] = { CODE_IAC, CODE_DONT, cCode };
		pClient->write( buf, 3 );
	}
	catch( Bu::HashException &e )
	{
		char buf[3] = { CODE_IAC, CODE_DONT, cCode };
		pClient->write( buf, 3 );
	}
}

void Bu::ProtocolTelnet::onSubOpt()
{
	switch( cSubOpt )
	{
		case OPT_NAWS:
			{
				uint16_t iWidth, iHeight;
				((char *)&iWidth)[1] = sSubBuf[0];
				((char *)&iWidth)[0] = sSubBuf[1];
				((char *)&iHeight)[1] = sSubBuf[2];
				((char *)&iHeight)[0] = sSubBuf[3];
				onSubNAWS( iWidth, iHeight );
			}
			break;

		default:
			onSubUnknown( cSubOpt, sSubBuf );
			break;
	}

	sSubBuf.clear();
}

void Bu::ProtocolTelnet::onCtlChar( char cChr )
{
#ifdef __TELNET_DEBUG 
	switch( cChr )
	{
		case CH_NUL:	printf("NUL ");	break;
		case CH_SOH:	printf("SOH ");	break;
		case CH_STX:	printf("STX ");	break;
		case CH_ETX:	printf("ETX ");	break;
		case CH_EOT:	printf("EOT ");	break;
		case CH_ENQ:	printf("ENQ ");	break;
		case CH_ACK:	printf("ACK ");	break;
		case CH_BEL:	printf("BEL ");	break;
		case CH_BS:		printf("BS ");	break;
		case CH_TAB:	printf("TAB ");	break;
		case CH_LF:		printf("LF ");	break;
		case CH_VT:		printf("VT ");	break;
		case CH_FF:		printf("FF ");	break;
		case CH_CR:		printf("CR ");	break;
		case CH_ESC:	printf("ESC "); break;
		case CH_DEL:	printf("DEL ");	break;
		default:		printf("!![%02x] ", cChr );	break;
	}
	fflush( stdout );
#endif

	switch( cChr )
	{
		case CH_DEL:
			{
				if( sDataBuf.getSize() > 0 )
				{
					sDataBuf.resize( sDataBuf.getSize()-1 );
					char buf[3] = { CH_BS, ' ', CH_BS };
					pClient->write( buf, 3 );
				}
			}
			break;

	}
}

#ifdef __TELNET_DEBUG
void Bu::ProtocolTelnet::printCode( char cCode )
{
	switch( cCode )
	{
		case CODE_SE:		printf("SE ");		break;
		case CODE_NOP:		printf("NOP ");		break;
		case CODE_DM:		printf("DM ");		break;
		case CODE_BRK:		printf("BRK ");		break;
		case CODE_IP:		printf("IP ");		break;
		case CODE_AO:		printf("AO ");		break;
		case CODE_AYT:		printf("AYT ");		break;
		case CODE_EC:		printf("EC ");		break;
		case CODE_EL:		printf("EL ");		break;
		case CODE_GA:		printf("GA ");		break;
		case CODE_SB:		printf("SB ");		break;
		case CODE_WILL:		printf("WILL ");	break;
		case CODE_WONT:		printf("WONT ");	break;
		case CODE_DO:		printf("DO ");		break;
		case CODE_DONT:		printf("DONT ");	break;
		case CODE_IAC:		printf("IAC ");		break;
		default:			printf("??%02x ", cCode );	break;
	}
	fflush( stdout );
}

void Bu::ProtocolTelnet::printOpt( char cOpt )
{
	switch( cOpt )
	{
		case OPT_BINARY:	printf("BINARY ");	break;
		case OPT_ECHO:		printf("ECHO ");	break;
		case OPT_SUPGA:		printf("SUPGA ");	break;
		case OPT_STATUS:	printf("STATUS ");	break;
		case OPT_TIMING:	printf("TIMING ");	break;
		case OPT_EXASCII:	printf("EXASCII ");	break;
		case OPT_LOGOUT:	printf("LOGOUT ");	break;
		case OPT_TTYPE:		printf("TTYPE ");	break;
		case OPT_NAWS:		printf("NAWS ");	break;
		case OPT_TSPEED:	printf("TSPEED ");	break;
		case OPT_NEWENV:	printf("NEWENV ");	break;
		case OPT_EXOPL:		printf("EXOPL ");	break;
		default:			printf("??%02x ", cOpt);	break;
	}
	fflush( stdout );
}
#endif

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

	char buf[3] = { CODE_IAC, 0, cCode };

	if( bSet )
	{
		buf[1] = CODE_WILL;
		rPT.pClient->write( buf, 3 );
#ifdef __TELNET_DEBUG
		printf("<= ");
		rPT.printCode( buf[0] );
		rPT.printCode( buf[1] );
		rPT.printOpt( buf[2] );
		printf("\n");
#endif
	}
	else
	{
		buf[1] = CODE_WONT;
		rPT.pClient->write( buf, 3 );
#ifdef __TELNET_DEBUG
		printf("<= ");
		rPT.printCode( buf[0] );
		rPT.printCode( buf[1] );
		rPT.printOpt( buf[2] );
		printf("\n");
#endif
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

void Bu::ProtocolTelnet::Option::remoteEnable( bool /*bSet*/ )
{
	return;
}

void Bu::ProtocolTelnet::Option::remoteSet( bool bSet )
{
	//if( bSet == (bool)(fOpts&fRemoteIs) ) return;

	char buf[3] = { CODE_IAC, 0, cCode };

	if( bSet )
	{
		buf[1] = CODE_DO;
		rPT.pClient->write( buf, 3 );
#ifdef __TELNET_DEBUG
		printf("<= ");
		rPT.printCode( buf[0] );
		rPT.printCode( buf[1] );
		rPT.printOpt( buf[2] );
		printf("\n");
#endif
	}
	else
	{
		buf[1] = CODE_DONT;
		rPT.pClient->write( buf, 3 );
#ifdef __TELNET_DEBUG
		printf("<= ");
		rPT.printCode( buf[0] );
		rPT.printCode( buf[1] );
		rPT.printOpt( buf[2] );
		printf("\n");
#endif
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

