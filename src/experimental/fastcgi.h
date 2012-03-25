/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FAST_CGI_H
#define BU_FAST_CGI_H

#include "bu/string.h"
#include "bu/hash.h"
#include "bu/array.h"
#include "bu/tcpsocket.h"
#include "bu/tcpserversocket.h"

namespace Bu
{
	class Stream;

	class FastCgi
	{
	public:
		FastCgi( int iPort );
		FastCgi();
		virtual ~FastCgi();

		static bool isEmbedded();

		typedef Bu::Hash<Bu::String, Bu::String> StrHash;
		enum RequestType
		{
			typeBeginRequest	= 1,
			typeAbortRequest	= 2,
			typeEndRequest		= 3,
			typeParams			= 4,
			typeStdIn			= 5,
			typeStdOut			= 6,
			typeStdErr			= 7,
			typeData			= 8,
			typeGetValues		= 9,
			typeGetValuesResult	= 10
		};

		enum Role
		{
			roleResponder		= 1,
			roleAuthorizer		= 2,
			roleFilter			= 3
		};

		enum Flags
		{
			flagsKeepConn		= 1
		};

		enum Status
		{
			statusRequestComplete	= 0,
			statusCantMpxConn		= 1,
			statusOverloaded		= 2,
			statusUnknownRole		= 3
		};

		typedef struct {
			uint8_t uVersion;
			uint8_t uType;
			uint16_t uRequestId;
			uint16_t uContentLength;
			uint8_t uPaddingLength;
			uint8_t uReserved;
		} Record;

		typedef struct {
			uint16_t uRole;
			uint8_t uFlags;
			uint8_t reserved[5];
		} BeginRequestBody;

		typedef struct {
			uint32_t uStatus;
			uint8_t uProtocolStatus;
			uint8_t reserved[3];
		} EndRequestBody;

		typedef struct Channel {
			Channel() : uFlags( 0 ) { }
			StrHash hParams;
			Bu::String sStdIn;
			Bu::String sData;
			uint8_t uFlags;
		} Channel;

		enum ChannelFlags
		{
			chflgParamsDone		= 0x01,
			chflgStdInDone		= 0x02,
			chflgDataDone		= 0x04,

			chflgAllDone		= 0x03
		};

		virtual void run();

		void stopRunning() { bRunning = false; }

		virtual void onInit() { };
		virtual int onRequest( const StrHash &hParams,
			const Bu::String &sStdIn, Bu::Stream &sStdOut,
			Bu::Stream &sStdErr )=0;
		virtual void onUninit() { };

	private:
		void read( Bu::TcpSocket &s, Record &r );
		void read( Bu::TcpSocket &s, BeginRequestBody &b );
		uint32_t readLen( Bu::TcpSocket &s, uint16_t &uUsed );
		void readPair( Bu::TcpSocket &s, StrHash &hParams, uint16_t &uUsed );

		void write( Bu::TcpSocket &s, Record r );
		void write( Bu::TcpSocket &s, EndRequestBody b );

		bool hasChannel( int iChan );

	private:
		Bu::TcpServerSocket *pSrv;
		bool bRunning;
		Bu::Array<Channel *> aChannel;
	};

	Bu::Formatter &operator<<( Bu::Formatter &f, const Bu::FastCgi::Record &r );
};

#endif
