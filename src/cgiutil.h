/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CGI_UTIL_H
#define CGI_UTIL_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/hash.h"

namespace Bu
{
	class CgiUtil
	{
	public:
		CgiUtil();
		virtual ~CgiUtil();

//		typedef Bu::List<Bu::FString> StrList;
//		typedef Bu::Hash<StrList> StrListHash;

//		static void parseUriQuery( const Bu::FString &sUri, StrListHash &hVar );

	private:
	};
};

#endif
