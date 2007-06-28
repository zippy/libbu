#ifndef BU_CLIENT_H
#define BU_CLIENT_H

#include <stdint.h>

#include "bu/fstring.h"

namespace Bu
{
	class Protocol;
	class Socket;

	/**
	 *
	 */
	class Client
	{
	public:
		Client( Bu::Socket *pSocket );
		virtual ~Client();

		void processInput();
		void processOutput();

		Bu::FString &getInput();
		Bu::FString &getOutput();
		void write( const char *pData, int nBytes );

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

		bool isOpen();

	private:
		Bu::Socket *pSocket;
		Bu::Protocol *pProto;
		Bu::FString sReadBuf;
		Bu::FString sWriteBuf;
	};
}

#endif
