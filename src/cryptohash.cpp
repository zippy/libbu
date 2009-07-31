/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/cryptohash.h"

Bu::CryptoHash::CryptoHash()
{
}

Bu::CryptoHash::~CryptoHash()
{
}

void Bu::CryptoHash::addData( const Bu::FString &sData )
{
	addData( sData.getStr(), sData.getSize() );
}

