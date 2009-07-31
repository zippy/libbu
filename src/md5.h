/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_MD5_H
#define BU_MD5_H

#include "bu/cryptohash.h"

namespace Bu
{
	/**
	 * Class for easily calculating MD5 sums of just about any data.
	 *@author Mike Buland
	 */
	class Md5 : public Bu::CryptoHash
	{
	public:
		/** Build an MD5 sum builder. */
		Md5();

		/** Deconstruct */
		virtual ~Md5();

		virtual void reset();
		virtual void setSalt( const Bu::FString &sSalt );
		virtual void addData( const void *sData, int iSize );
		using Bu::CryptoHash::addData;
		virtual FString getResult();

	private:
		/**
		 * Compute one block of input data.
		 */
		void compBlock( long *x, long *lsum );
		
		long inbuf[16];
		long iFill;
		long sum[4];
		uint64_t iBytes;
	};
};

#endif
