/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROTOCOL_TELNET_H
#define BU_PROTOCOL_TELNET_H

#include "bu/protocol.h"
#include "bu/hash.h"
#include "bu/string.h"

// #define __TELNET_DEBUG

namespace Bu
{
	/**
	 * Telnet Protocol handler.  This attempts to provide useful and general
	 * support for most of the most commonly used Telnet extensions in a simple
	 * and easy to use way.  The Option variables control the settings that can
	 * be used on the line, and control which virtual "callbacks" will be called
	 * when different events happen.
	 *
	 * To setup initial values and to disable any options you wish override the
	 * onNewConnection function in your own class, like this:
	 *@code
	 class MyTelnet : public Bu::ProtocolTelnet
	 {
	 public:
	 	...

		virtual void onNewConnection( class Bu::Client *pClient )
		{
			// Call the parent class' onNewConnection to get everything all
			// set up.
			Bu::ProtocolTelnet::onNewConnection( pClient );

			// These functions disable the option to send files via telnet,
			// disabling the remote option means that we won't accept this
			// option (binary data being sent to us) from the client.
			//
			// Disabling the local option means that the client cannot ask us
			// to send them binary data.
			oBinary.enableRemote( false );
			oBinary.enableLocal( false );

			// This requests that the client send us window size updates
			// whenever the size of their window changes, and an initial set to
			// boot.
			//
			// To see if this option is set later, try oNAWS.isRemoteSet(), but
			// wait a little while, asking immediatly will always return false,
			// since the remote side has yet to receive our request.
			oNAWS.remoteSet();
		}
	}
	 @endcode
	 *
	 *@ingroup Serving
	 */
	class ProtocolTelnet : public Protocol
	{
	public:
		ProtocolTelnet();
		virtual ~ProtocolTelnet();

		/**
		 * If you override this function in a child class, make sure to call
		 * this version of it as the very first thing that you do, before you
		 * set any options.  See the example in the class docs.
		 */
		virtual void onNewConnection( class Bu::Client *pClient );

		/**
		 * You should never override this function unless you really, really
		 * know what you're doing.  If you want to get data after each line
		 * entered (in canonical mode) or after any data arrives (non canonical
		 * mode) then override the gotLine and gotData functions, respectively.
		 */
		virtual void onNewData( class Bu::Client *pClient );

		/**
		 * Override this function to be notified of lines being submitted by
		 * the client.  This function is only called in canonical mode, after
		 * all edits are performed on the data.  In this mode weather you use
		 * the line or not, the data will be cleared from the buffer when this
		 * function returns, any changes made to the buffer will be destroyed.
		 */
		virtual void gotLine( Bu::String & /*sLine*/ ){};

		/**
		 * Override this function to be notified of any new data that comes in
		 * from the client.  This function is only called in non-canonical mode,
		 * and includes all raw data minus telnet control codes and ansi
		 * escape sequences.  In this mode control of the buffer is up to the
		 * child class in this function, the buffer will never be cleared unless
		 * it happens in this function's override.
		 */
		virtual void gotData( Bu::String & /*sData*/ ){};

		/**
		 * Using this function to enable or disable canonical mode only affects
		 * the way the data is processed and which virtual functions are called
		 * during processing.  It does not affect options set locally or
		 * remotely.  Setting this to false will enable char-at-a-time mode,
		 * effectively disabling internal line-editing code.  Characters
		 * such as backspace that are detected will not be handled and will be
		 * sent to the user override.  The subclass will also be notified every
		 * time new data is available, not just whole lines.
		 *
		 * When set to true (the default), line editing control codes will be
		 * interpreted and used, and the subclass will only be notified when
		 * complete lines are available in the buffer.
		 */
		void setCanonical( bool bCon=true );
		bool isCanonical();

		void write( const Bu::String &sData );
		void write( const char *pData, int iSize );
		void write( char cData );

		const Bu::String &getBuffer() { return sDataBuf; }

	public:
		/**
		 * If you wish to know the current dimensions of the client window,
		 * override this function, it will be called whenever the size changes.
		 */
		virtual void onSubNAWS( uint16_t /*iWidth*/, uint16_t /*iHeight*/ ){};

		/**
		 * This function is called whenever an unknown sub negotiation option is
		 * sent over the line.  This doesn't mean that it's malformatted, it
		 * just means that this class doesn't support that option yet, but you
		 * can handle it yourself if you'd like.  Feel free to change the
		 * sSubBuf, it will be cleared as soon as this function returns anyway.
		 */
		virtual void onSubUnknown( char /*cSubOpt*/,
			Bu::String & /*sSubBuf*/ ){};

	private:
		/**
		 * Represents a basic telnet option, either on or off, no parameters.
		 * Each Option can negotiate effectively on it's own, and has two
		 * parameters in each of two classes.  Both local and remote can be
		 * enabled/disabled and set/unset.  Enabled represents the ability to
		 * set the option, disabling an option should also unset it.  Set or
		 * unset represent wether the option is being used, if it is allowed.
		 */
		class Option
		{
			friend class Bu::ProtocolTelnet;
		private:
			Option( ProtocolTelnet &rPT, char cCode );
			virtual ~Option();

		public:
			void localEnable( bool bSet=true );
			void localSet( bool bSet=true );
			
			bool isLocalEnabled();
			bool isLocalSet();
			
			void remoteEnable( bool bSet=true );
			void remoteSet( bool bSet=true );

			bool isRemoteEnabled();
			bool isRemoteSet();

		private:
			enum
			{
				fLocalCant	= 0x01,	/**< Local can't/won't allow option. */
				fLocalIs	= 0x02, /**< Local is using option. */
				fRemoteCant	= 0x04, /**< Remote can't/won't allow option. */
				fRemoteIs	= 0x08	/**< Remote is using option. */
			};

			ProtocolTelnet &rPT;
			char fOpts;
			char cCode;
		};
		friend class Bu::ProtocolTelnet::Option;

		Hash<char, Option *> hOpts;

	public:
		Option oBinary;
		Option oEcho;
		Option oNAWS;
		Option oSuppressGA;

	private:
		void onWill( char cCode );
		void onWont( char cCode );
		void onDo( char cCode );
		void onDont( char cCode );
		void onSubOpt();
		void onCtlChar( char cChr );

#ifdef __TELNET_DEBUG
		void printCode( char cCode );
		void printOpt( char cOpt );
#endif

	private:
		Client *pClient;

		Bu::String sDataBuf;	/**< Buffer for regular line data. */
		Bu::String sSubBuf;	/**< Buffer for subnegotiation data. */
		char cSubOpt;	/**< Which suboption are we processing. */

		bool bCanonical;	/**< Are we canonicalizing incoming data? */
		bool bSubOpt;		/**< Are we processing a suboption right now? */
	};
}

#endif
