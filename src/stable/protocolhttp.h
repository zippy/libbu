/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROTOCOL_HTTP_H
#define BU_PROTOCOL_HTTP_H

#include <stdint.h>
#include <sys/types.h>

#include "bu/protocol.h"
#include "bu/client.h"
#include "bu/string.h"
#include "bu/hash.h"

namespace Bu
{
    /**
     * An HTTP Protocol handler.  Yes, I know that HTTP stands for Hyper Text
     * Transfer Protocol, and that the Protocol part is redundant, but in this
     * case the word Protocol is refering to the Libbu++ construct Bu::Protocol,
     * and not a means of encoding conversations.  Anyway, this class represents
     * a general HTTP server processor.  Every time a request comes in it calls
     * the onRequest function in a subclass with the method and URI that were
     * requested.  The sub-class can then do whatever it needs to to send back
     * a response.
     *@ingroup Serving
     */
    class ProtocolHttp : public Protocol
    {
    public: /* Types */
        typedef Bu::List<Bu::String> TokenList;

    public: /* Interface */
        ProtocolHttp();
        virtual ~ProtocolHttp();

        virtual void onNewConnection( Bu::Client *pClient );
        virtual void onNewData( Bu::Client *pClient );

        virtual void onRequest(
            const Bu::String &sMethod, const Bu::String &sPath )=0;

        class Response
        {
            friend class Bu::ProtocolHttp;
        public:
            Response( int iCode );
            Response( int iCode, const Bu::String &sReason );
            virtual ~Response();

            void setHeader( const Bu::String &sKey, const Bu::String &sVal );
            void setContent( const Bu::String &sCont );

        private:
            int iCode;
            Bu::String sReason;
            typedef Bu::Hash<Bu::String,Bu::String> StringHash;
            StringHash hHeaders;
            Bu::String sContent;
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
        TokenType getToken( Bu::String &line );
        bool isWS( char buf );
        bool isSeperator( char buf );

        void earlyResponse();
        void lateResponse();

    private: /* state */
        Bu::Client *pClient;
        TokenList lTokens;

        int iState;

        Bu::String sMethod;
        Bu::String sPath;
        int iMajor;
        int iMinor;
    };
}

#endif
