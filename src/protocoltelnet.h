#ifndef BU_PROTOCOL_TELNET_H
#define BU_PROTOCOL_TELNET_H

#include "bu/protocol.h"
#include "bu/hash.h"

namespace Bu
{
	class ProtocolTelnet : public Protocol
	{
	public:
		ProtocolTelnet();
		virtual ~ProtocolTelnet();

		virtual void onNewConnection( class Bu::Client *pClient );
		virtual void onNewData( class Bu::Client *pClient );

		enum OptMode
		{
			optOff,
			optOn,
			optDesire,
			optRefuse
		};

		OptMode getLocalOptBinary();
		void setLocalOptBinary( OptMode eMode );
		OptMode getRemoteOptBinary();
		void setRemoteOptBinary( OptMode eMode );

		OptMode getLocalOptEcho();
		void setLocalOptEcho( OptMode eMode );
		OptMode getRemoteOptEcho();
		void setRemoteOptEcho( OptMode eMode );

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

		Hash<char, Option *> hOpts;

		Option oBinary;
		Option oEcho;

		Client *pClient;

	friend class Bu::ProtocolTelnet::Option;
	};
}

#endif
