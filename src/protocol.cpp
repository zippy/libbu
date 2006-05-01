/***************************************************************************
 *   Copyright (C) 2003 by Mike Buland                                     *
 *   eichlan@yf-soft.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "protocol.h"

Protocol::Protocol()
{
	pConnection = NULL;
}

Protocol::~Protocol()
{
}

void Protocol::setConnection( Connection *pNewConnection )
{
	pConnection = pNewConnection;

	onNewConnection();
}

Connection *Protocol::getConnection()
{
	return pConnection;
}
