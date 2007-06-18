#ifndef CLIENT_H
#define CLIENT_H

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

		Bu::FString &getInput();

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

	private:
		Bu::Socket *pSocket;
		Bu::Protocol *pProto;
		Bu::FString sReadBuf;
	};
}

#endif
