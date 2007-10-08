#ifndef BU_PROTOCOL_HTTP_H
#define BU_PROTOCOL_HTTP_H

#include <stdint.h>
#include <sys/types.h>

#include "bu/protocol.h"
#include "bu/client.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 *
	 */
	class ProtocolHttp : public Protocol
	{
	public: /* Types */
		typedef Bu::List<Bu::FString> TokenList;

	public: /* Interface */
		ProtocolHttp();
		virtual ~ProtocolHttp();

		virtual void onNewConnection( Bu::Client *pClient );
		virtual void onNewData( Bu::Client *pClient );

		virtual void onRequest(
			const Bu::FString &sMethod, const Bu::FString &sPath )=0;

		class Response
		{
			friend class Bu::ProtocolHttp;
		public:
			Response( int iCode );
			Response( int iCode, const Bu::FString &sReason );
			virtual ~Response();

			void setHeader( const Bu::FString &sKey, const Bu::FString &sVal );
			void setContent( const Bu::FString &sCont );

		private:
			int iCode;
			Bu::FString sReason;
			typedef Bu::Hash<Bu::FString,Bu::FString> StringHash;
			StringHash hHeaders;
			Bu::FString sContent;
		};
		
		void sendResponse( const Response &rRes );
	
	private:
		enum TokenType
		{
			ttOutOfData,
			ttString,
			ttNewline,
			ttDoubleNewline,
			ttSeperator
		};
		/**
		 * Read an HTTP line, this is up to the first CRLF that isn't followed
		 * by a continuation character, converting it to one line as it reads.
		 *@param line All data read will be appended to line, even if no
		 * end-of-line is read.
		 *@returns True if an end-of-line is read and the line should be
		 * processed, false if the end-of-line has not been reached, and more
		 * data needs to be read before this operation can continue.
		 */
		TokenType getToken( Bu::FString &line );
		bool isWS( char buf );
		bool isSeperator( char buf );

		void earlyResponse();
		void lateResponse();

	private: /* state */
		Bu::Client *pClient;
		TokenList lTokens;

		int iState;

		Bu::FString sMethod;
		Bu::FString sPath;
		int iMajor;
		int iMinor;
	};
}

#endif
