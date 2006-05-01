#include "protocoltelnet.h"
#include <string.h>

ProtocolTelnet::ProtocolTelnet()
{
	nTermType = termUnInited;
	bEchoOn = true;
}

ProtocolTelnet::~ProtocolTelnet()
{
}

bool ProtocolTelnet::onNewConnection()
{
	Connection *pCon = getConnection();

	pCon->appendOutput( (char)IAC );
	pCon->appendOutput( (char)WILL );
	pCon->appendOutput( (char)SUPPRESSGA );

	pCon->appendOutput( (char)IAC );
	pCon->appendOutput( (char)DO );
	pCon->appendOutput( (char)SUPPRESSGA );

	pCon->appendOutput( (char)IAC );
	pCon->appendOutput( (char)DONT );
	pCon->appendOutput( (char)TERMTYPE );

//	pCon->appendOutput( IAC );
//	pCon->appendOutput( SB );
//	pCon->appendOutput( TERMTYPE );
//	pCon->appendOutput( 1 );
//	pCon->appendOutput( IAC );
//	pCon->appendOutput( SE );

	pCon->appendOutput( (char)IAC );
	pCon->appendOutput( (char)DONT );
	pCon->appendOutput( (char)ECHO );

	pCon->appendOutput( (char)IAC );
	pCon->appendOutput( (char)WILL );
	pCon->appendOutput( (char)ECHO );

//	255(IAC),251(WILL),3
}

bool ProtocolTelnet::onNewData()
{
	Connection *pCon = getConnection();
	if( !pCon->hasInput() )
	{
	    return true;
	}

	int nInSize = pCon->getInputAmnt();
	char *lpInStr = (char *)pCon->getInput();

	// Here we interpret the basic commands and un-encapsulate them, so to
	// speak.  We'll allow this, even if the terminal is in raw mode, we
	// just won't send anything in response...
	for( int j = 0; j < nInSize; j++ )
	{
		switch( (unsigned char)lpInStr[j] )
		{
		case '\r':
			fbEdited.appendData('\n');
			if( bEchoOn ) pCon->appendOutput("\n\r");
			break;
		
		case '\n':
			break;

		case '\177': // backspace
			if( fbEdited.getLength() > 0 )
			{
				fbEdited.usedData( -1 );  // Delete one char from the end
				if( bEchoOn ) pCon->appendOutput(ESC "[D"); // Move the cursor back one
				if( bEchoOn ) pCon->appendOutput(ESC "[P"); // Delete one character
			}
			break;

		case '\x1B': // escape sequence
			if( (unsigned char)lpInStr[j+1] == '[' )
			{
				switch( (unsigned char)lpInStr[j+2] )
				{
					case 'A': // Up
						break;

					case 'B': // Down
						break;

					case 'C': // Right
						break;

					case 'D': // Left
						break;
				}
				j+=2;
			}
			break;

		case 0:   // NOP: No operation
			break;

		case IAC: // IAC: Interpret as command
			switch( lpInStr[j+1] )
			{
			case SE: // SE: End of subnegotiation parameters.
				break;

			case NOP: // NOP: No operation
				break;

			case DM: // DM: Data mark. Indicates the position of a Synch event within the data stream. This should always be accompanied by a TCP urgent notification.
				break;

			case BRK: // BRK: Break. Indicates that the "break" or "attention" key was hit.
				break;

			case IP: // IP: Suspend, interrupt or abort the process to which the NVT is connected.
				break;

			case AO: // AO: Abort output. Allows the current process to run to completion but do not send its output to the user.
				break;

			case AYT: // AYT: Are you there. Send back to the NVT some visible evidence that the AYT was received.
				break;

			case EC: // EC: Erase character. The receiver should delete the last preceding undeleted character from the data stream.
				break;

			case EL: // EL: Erase line. Delete characters from the data stream back to but not including the previous CRLF.
				break;

			case GA: // GA: Go ahead. Used, under certain circumstances, to tell the other end that it can transmit.
				break;

			case SB: // SB: Subnegotiation of the indicated option follows.
				switch( lpInStr[j+2] )
				{
				case TERMTYPE:
					if( lpInStr[j+3] == 0 )
					{
						for( int k = 0; j+4+k < nInSize; k++ )
						{
							if( (unsigned char)lpInStr[j+4+k] == IAC &&
								(unsigned char)lpInStr[j+5+k] == SE )
							{
								lpInStr[j+4+k] = 0;
								//@TODO:  Do something with the term type...
								printf("Term type:  %s\n", &lpInStr[j+4] );
								j += 5+k;
							}
						}
					}
					else
					{
					}
					break;
					
				default:
					//printf("unknown subnegotiation parameters! (%d)\n", lpInStr[j+2] );
					break;
				}
				break;

			case WILL: // WILL: Indicates the desire to begin performing
				switch( lpInStr[j+2] )
				{
				case SUPPRESSGA:
					j += 2;
//					pCon->usedInput( 3 );
					break;

				case TERMTYPE:
					j += 2;
//					pCon->usedInput( 3 );
					break;

				case ECHO:
					j += 2;
//					pCon->usedInput( 3 );
					break;
					
				case NAWS:
				default:
					pCon->appendOutput( (char)ESC[0] );
					pCon->appendOutput( (char)DONT );
					pCon->appendOutput( lpInStr[j+2] );
					//printf("unknown will command used! (%d)\n", lpInStr[j+2] );
					j += 2;
					break;
				}
				break;

			case WONT: // WONT: Indicates the refusal to perform
				switch( lpInStr[j+2] )
				{
				case ECHO:
					j += 2;
//					pCon->usedInput( 3 );
					break;
					
				default:
					//printf("unknown wont command used! (%d)\n", lpInStr[j+2] );
					j += 2;
					break;
				}
				break;

			case DO: // DO: Indicates the request that the other party perform
				switch( lpInStr[j+2] )
				{
				case ECHO:
					j += 2;
					break;
					
				case SUPPRESSGA:
					j += 2;
					break;
					
				default:
					pCon->appendOutput( (char)ESC[0] );
					pCon->appendOutput( (char)DONT );
					pCon->appendOutput( lpInStr[j+2] );
					//printf("unknown do command used! (%d)\n", lpInStr[j+2] );
					j += 2;
					break;
				}
//				pCon->usedInput( 3 );
				break;

			case DONT: // DONT: Indicates the demand that the other party stop performing
				switch( lpInStr[j+2] )
				{
				case ECHO:
					j += 2;
//					pCon->usedInput( 3 );
					break;
				
				default:
					printf("unknown dont command used! (%d)\n", lpInStr[j+2] );
					j += 2;
					break;
				}
				break;
			}
			break;

		default:
			fbEdited.appendData( lpInStr[j] );
			if( bEchoOn ) pCon->appendOutput( lpInStr[j] );
			break;
		}
	}

	pCon->usedInput( pCon->getInputAmnt() );

    return true;
}

char *ProtocolTelnet::getLine( bool bFullOnly )
{
	int i = fbEdited.findChar('\n');

	if( i < 0 )
	{
		if( bFullOnly == false )
		{
			i = fbEdited.getLength();
		}
		else
		{
			return NULL;
		}
	}

	char *lpStr = new char[i+1];
	strncpy( lpStr, fbEdited.getData(), i );
	lpStr[i] = '\0';

	fbEdited.usedData( i+1 );

	return lpStr;
}

char *ProtocolTelnet::peekLine( bool bFullOnly )
{
	int i = fbEdited.findChar('\n');

	if( i < 0 )
	{
		if( bFullOnly == false )
		{
			i = fbEdited.getLength();
		}
		else
		{
			return NULL;
		}
	}

	char *lpStr = new char[i+1];
	strncpy( lpStr, fbEdited.getData(), i );
	lpStr[i] = '\0';

	return lpStr;
}

void ProtocolTelnet::setEcho( bool bEchoOn )
{
	this->bEchoOn = bEchoOn;
}
