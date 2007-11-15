/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROTOCOL_H
#define BU_PROTOCOL_H

#include <stdint.h>

namespace Bu
{
	class Client;

	/**
	 *@author Mike Buland
	 *@ingroup Serving
	 */
	class Protocol
	{
	public:
		Protocol();
		virtual ~Protocol();

		virtual void onNewConnection( Bu::Client *pClient )=0;
		virtual void onNewData( Bu::Client *pClient )=0;

	private:

	};
}

#endif
