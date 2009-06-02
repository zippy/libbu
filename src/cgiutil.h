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
