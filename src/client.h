/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CLIENT_H
#define BU_CLIENT_H

#include <stdint.h>

#include "bu/fstring.h"

namespace Bu
{
	class Protocol;
	class Socket;
	class ClientLinkFactory;

	/**
	 *@author Mike Buland
	 *@ingroup Serving
	 */
	class Client
	{
	public:
		Client( Bu::Socket *pSocket, Bu::ClientLinkFactory *pfLink );
		virtual ~Client();

		void processInput();
		void processOutput();

		Bu::FString &getInput();
		Bu::FString &getOutput();
		void write( const Bu::FString &sData );
		void write( const void *pData, int nBytes );
		void write( int8_t nData );
		void write( int16_t nData );
		void write( int32_t nData );
		void write( int64_t nData );
		void write( uint8_t nData );
		void write( uint16_t nData );
		void write( uint32_t nData );
		void write( uint64_t nData );
		void read( void *pData, int nBytes );
		void peek( void *pData, int nBytes, int nOffset=0 );
		void seek( int nBytes );
		long getInputSize();

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

		bool isOpen();
		void close();

		const Bu::Socket *getSocket() const;

		void disconnect();
		bool wantsDisconnect();

		class ClientLink *getLink();

		void onMessage( const Bu::FString &sMsg );

	private:
		Bu::Socket *pSocket;
		Bu::Protocol *pProto;
		Bu::FString sReadBuf;
		int nRBOffset;
		Bu::FString sWriteBuf;
		bool bWantsDisconnect;
		class Bu::ClientLinkFactory *pfLink;
	};
}

#endif
