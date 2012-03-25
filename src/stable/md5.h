/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
	 * This code is based on some public domain code written by Colin Plumb in
	 * 1993.
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
		virtual void setSalt( const Bu::String &sSalt );
		virtual void addData( const void *sData, int iSize );
		using Bu::CryptoHash::addData;
		virtual String getResult();
		virtual void writeResult( Bu::Stream &sOut );

	private:
		/**
		 * Compute one block of input data.
		 */
		void compBlock( uint32_t *lsum, uint32_t *x );
		void compCap( uint32_t *sumout );

		void _addData( uint8_t *target, int &iCurFill, const void *sData,
				int iSize );
		void _toLittleEndian( uint8_t *buf, uint32_t count );
		
		uint8_t inbuf[64];
		uint32_t sum[4];
		uint32_t uBits[2];
	};
};

#endif
