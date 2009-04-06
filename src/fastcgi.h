#ifndef BU_FAST_CGI_H
#define BU_FAST_CGI_H

#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/array.h"
#include "bu/socket.h"
#include "bu/serversocket.h"

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

		typedef Bu::Hash<Bu::FString, Bu::FString> StrHash;
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
			Bu::FString sStdIn;
			Bu::FString sData;
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

		virtual void init() { };
		virtual int request( const StrHash &hParams,
			const Bu::FString &sStdIn, Bu::Stream &sStdOut,
			Bu::Stream &sStdErr )=0;
		virtual void deinit() { };

	private:
		void read( Bu::Socket &s, Record &r );
		void read( Bu::Socket &s, BeginRequestBody &b );
		uint32_t readLen( Bu::Socket &s, uint16_t &uUsed );
		void readPair( Bu::Socket &s, StrHash &hParams, uint16_t &uUsed );

		void write( Bu::Socket &s, Record r );
		void write( Bu::Socket &s, EndRequestBody b );

	private:
		Bu::ServerSocket *pSrv;
		bool bRunning;
		Bu::Array<Channel *> aChannel;
	};
};

#endif
