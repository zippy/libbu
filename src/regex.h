/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_REG_EX_H
#define BU_REG_EX_H

#include "bu/string.h"

#include <stdint.h>

namespace Bu
{
	class RegEx
	{
	public:
		RegEx();
		RegEx( const Bu::String &sSrc );
		virtual ~RegEx();

		void compile( const Bu::String &sSrc );
		int getNumSubStrings();
		bool execute( const Bu::String &sSrc );
		void getSubStringRange( int nIndex, int &iStart, int &iEnd );
		Bu::String getSubString( int nIndex );
		const Bu::String &getSource()
		{
			return sSrc;
		}

	private:
		Bu::String sSrc;
		Bu::String sTest;
		void *pRegEx;
		bool bCompiled;
		int nSubStr;
		void *paSubStr;
	};
};

#endif
