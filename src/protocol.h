#ifndef BU_PROTOCOL_H
#define BU_PROTOCOL_H

#include <stdint.h>

namespace Bu
{
	class Client;

	/**
	 *
	 */
	class Protocol
	{
	public:
		Protocol();
		virtual ~Protocol();

		virtual void onNewData( Bu::Client *pClient )=0;

	private:

	};
}

#endif
