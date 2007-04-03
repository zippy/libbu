#ifndef PROTOCOLTELNET_H
#define PROTOCOLTELNET_H

#include "protocol.h"
#include "flexbuf.h"

#define ESC		"\x1B"		/**< A telnet escape code. */

/** Handles all specialized protocol actions related to the telnet protocol.
  * This includes setting modes, non-scrollable regions, and so on.
  *@author Mike Buland
  */
class ProtocolTelnet : public Protocol
{
public:
	ProtocolTelnet();
	virtual ~ProtocolTelnet();

    bool onNewData();
	bool onNewConnection();

	char *getLine( bool bFullOnly = true );
	char *peekLine( bool bFullOnly = true );
	
	void setEcho( bool bEchoOn = true );
	
	enum
	{
		termUnInited,
		termRaw,
		termUnknown,
		termVT220,
		termXTerm
	};

	enum
	{
		SE   = 240, // SE: End of subnegotiation parameters.
		NOP  = 241, // NOP: No operation
		DM   = 242, // DM: Data mark. Indicates the position of a Synch event within the data stream. This should always be accompanied by a TCP urgent notification.
		BRK  = 243, // BRK: Break. Indicates that the "break" or "attention" key was hit.
		IP   = 244, // IP: Suspend, interrupt or abort the process to which the NVT is connected.
		AO   = 245, // AO: Abort output. Allows the current process to run to completion but do not send its output to the user.
		AYT  = 246, // AYT: Are you there. Send back to the NVT some visible evidence that the AYT was received.
		EC   = 247, // EC: Erase character. The receiver should delete the last preceding undeleted character from the data stream.
		EL   = 248, // EL: Erase line. Delete characters from the data stream back to but not including the previous CRLF.
		GA   = 249, // GA: Go ahead. Used, under certain circumstances, to tell the other end that it can transmit.
		SB   = 250, // SB: Subnegotiation of the indicated option follows.
		WILL = 251, // WILL: Indicates the desire to begin performing, or confirmation that you are now performing, the indicated option.
		WONT = 252, // WONT: Indicates the refusal to perform, or continue performing, the indicated option.
		DO   = 253, // DO: Indicates the request that the other party perform, or confirmation that you are expecting the other party to perform, the indicated option.
		DONT = 254, // DONT: Indicates the demand that the other party stop performing, or confirmation that you are no longer expecting the other party to perform, the indicated option.
		IAC  = 255 // IAC: Interpret as command
	};

	enum
	{
		ECHO = 1,			// Explain who'll echo
		SUPPRESSGA = 3,		// Suppress Go Ahead
		TERMTYPE = 24,  	// Terminal Type
		NAWS = 31,			// Window size
		TERMSPEED = 32,		// Terminal Speed
		LINEMODE = 34		// Linemode
	};

private:
	int nTermType;

	int nTermWidth;
	int nTermHeight;

	FlexBuf fbEdited;
	
	bool bEchoOn;
};

#endif
