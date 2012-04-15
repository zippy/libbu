/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROTOCOL_H
#define BU_PROTOCOL_H

#include <stdint.h>

#include "bu/string.h"

namespace Bu
{
	class Client;

	/**
	 *@ingroup Serving
	 */
	class Protocol
	{
	public:
		Protocol();
		virtual ~Protocol();

		virtual void onNewConnection( Bu::Client *pClient );
		virtual void onNewData( Bu::Client *pClient );
		virtual void onMessage( Bu::Client *pClient, const Bu::String &sMsg );
		virtual void onTick( Bu::Client *pClient );

	private:

	};
}

#endif
