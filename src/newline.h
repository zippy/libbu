/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_NEW_LINE_H
#define BU_NEW_LINE_H

#include "bu/filter.h"

namespace Bu
{
	/**
	 * Converts new-line characters from any standard convention into linefeeds
	 * (\n) on reading, and converts them to either your OS's standard or a
	 * specified standard, depending on how you construct the class.
	 *
	 * If you're reading in a text file, then this filter is practically
	 * required.
	 */
	class NewLine : public Bu::Filter
	{
	public:
		NewLine( Bu::Stream &rNext );
		virtual ~NewLine();

		virtual void start();
		virtual size_t stop();

		virtual size_t read( void *pBuf, size_t iAmnt );
		virtual size_t write( const void *pBuf, size_t iAmnt );

	private:
		bool bExChar;
		char cExChar;
	};
};

#endif
