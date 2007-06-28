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
		void write( int8_t nData );
		void write( int16_t nData );
		void write( int32_t nData );
		void write( int64_t nData );
		void read( char *pData, int nBytes );
		long getInputSize();

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

		bool isOpen();

		const Bu::Socket *getSocket() const;

	private:
		Bu::Socket *pSocket;
		Bu::Protocol *pProto;
		Bu::FString sReadBuf;
		int nRBOffset;
		Bu::FString sWriteBuf;
	};
}

#endif
