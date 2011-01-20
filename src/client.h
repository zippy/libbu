/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CLIENT_H
#define BU_CLIENT_H

#include <stdint.h>

#include "bu/config.h"
#include "bu/string.h"
#include "bu/queuebuf.h"

namespace Bu
{
	class Protocol;
	class Stream;
	class TcpSocket;
	class ClientLinkFactory;

	/**
	 *@ingroup Serving
	 */
	class Client : public Bu::Stream
	{
	public:
		Client( Bu::TcpSocket *pSocket, Bu::ClientLinkFactory *pfLink );
		virtual ~Client();

		void processInput();
		void processOutput();

		//Bu::String &getInput();
		//Bu::String &getOutput();
		Bu::size write( const Bu::String &sData );
		Bu::size write( const void *pData, Bu::size nBytes );
		Bu::size write( int8_t nData );
		Bu::size write( int16_t nData );
		Bu::size write( int32_t nData );
		Bu::size write( int64_t nData );
		Bu::size write( uint8_t nData );
		Bu::size write( uint16_t nData );
		Bu::size write( uint32_t nData );
		Bu::size write( uint64_t nData );
		Bu::size read( void *pData, Bu::size nBytes );
		Bu::size peek( void *pData, int nBytes, int nOffset=0 );
//		void seek( int nBytes );
		Bu::size getInputSize();
		Bu::size getOutputSize();

		void setProtocol( Protocol *pProto );
		Bu::Protocol *getProtocol();
		void clearProtocol();

		bool isOpen();
		void close();
		void tick();

		const Bu::TcpSocket *getSocket() const;

		void disconnect();
		bool wantsDisconnect();

		class ClientLink *getLink();

		void onMessage( const Bu::String &sMsg );

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

		/*
		 * These are required to qualify as a stream, I dunno how many will
		 * be implemented.
		 */
		virtual Bu::size tell();
		virtual void seek( Bu::size offset );
		virtual void setPos( Bu::size pos );
		virtual void setPosEnd( Bu::size pos );
		virtual bool isEos();
		virtual void flush();
		virtual bool canRead();
		virtual bool canWrite();
		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();
		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );
		virtual void setSize( Bu::size iSize );
		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

	private:
		typedef Bu::List<Bu::Stream *> FilterList;
		FilterList lFilts;
		Bu::Stream *pTopStream;
		Bu::TcpSocket *pSocket;
		Bu::Protocol *pProto;
		Bu::QueueBuf qbRead;
		Bu::QueueBuf qbWrite;
		bool bWantsDisconnect;
		class Bu::ClientLinkFactory *pfLink;
	};
}

#endif
