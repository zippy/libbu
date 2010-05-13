/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CLIENT_H
#define BU_CLIENT_H

#include <stdint.h>

#include "bu/fstring.h"
#include "bu/queuebuf.h"

namespace Bu
{
	class Protocol;
	class Stream;
	class Socket;
	class ClientLinkFactory;

	/**
	 *@ingroup Serving
	 */
	class Client
	{
	public:
		Client( Bu::Socket *pSocket, Bu::ClientLinkFactory *pfLink );
		virtual ~Client();

		void processInput();
		void processOutput();

		//Bu::FString &getInput();
		//Bu::FString &getOutput();
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
		int read( void *pData, int nBytes );
		int peek( void *pData, int nBytes, int nOffset=0 );
		void seek( int nBytes );
		long getInputSize();
		long getOutputSize();

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

		bool isOpen();
		void close();
		void tick();

		const Bu::Socket *getSocket() const;

		void disconnect();
		bool wantsDisconnect();

		class ClientLink *getLink();

		void onMessage( const Bu::FString &sMsg );

		bool hasOutput() { return qbWrite.getSize() > 0; }
		bool hasInput() { return qbRead.getSize() > 0; }

		template<typename filter>
		void pushFilter()
		{
			filter *pFlt = new filter( *pTopStream );
			pTopStream = pFlt;
			lFilts.prepend( pFlt );
		}
		
		template<typename filter, typename p1t>
		void pushFilter( p1t p1 )
		{
			filter *pFlt = new filter( *pTopStream, p1 );
			pTopStream = pFlt;
			lFilts.prepend( pFlt );
		}
		
		template<typename filter, typename p1t, typename p2t>
		void pushFilter( p1t p1, p2t p2 )
		{
			filter *pFlt = new filter( *pTopStream, p1, p2 );
			pTopStream = pFlt;
			lFilts.prepend( pFlt );
		}

	private:
		typedef Bu::List<Bu::Stream *> FilterList;
		FilterList lFilts;
		Bu::Stream *pTopStream;
		Bu::Socket *pSocket;
		Bu::Protocol *pProto;
		Bu::QueueBuf qbRead;
		Bu::QueueBuf qbWrite;
		bool bWantsDisconnect;
		class Bu::ClientLinkFactory *pfLink;
	};
}

#endif
