/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CRYPTO_HASH_H
#define BU_CRYPTO_HASH_H

#include "bu/fstring.h"

namespace Bu
{
	class CryptoHash
	{
	public:
		CryptoHash();
		virtual ~CryptoHash();

		virtual void reset() = 0;
		virtual void setSalt( const Bu::FString &sSalt ) = 0;
		virtual void addData( const void *sData, int iSize ) = 0;
		virtual void addData( const Bu::FString &sData );
		virtual FString getResult() = 0;
	};
};

#endif
