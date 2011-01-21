/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PEARSON_HASH_H
#define BU_PEARSON_HASH_H

#include "bu/cryptohash.h"

namespace Bu
{
	/**
	 * A pearson hash is a non-cryptographically secure hashing function that
	 * is very light on resources, very fast, and produces a single byte
	 * as it's output.  It is strongly dependant on every byte in the input,
	 * which means that it's a good choice for adding to short messages to
	 * ensure that the contents of the messages are unchanged.
	 */
	class PearsonHash : public Bu::CryptoHash
	{
	public:
		PearsonHash();
		virtual ~PearsonHash();

		virtual void reset();
		virtual void setSalt( const Bu::String &sSalt );
		virtual void addData( const void *sData, int iSize );
		using Bu::CryptoHash::addData;
		virtual String getResult();
		virtual void writeResult( Stream &sOut );

	private:
		static uint8_t aSBox[256];
		uint8_t iValue;
	};
};

#endif
