/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CSV_READER_H
#define BU_CSV_READER_H

#include "bu/fstring.h"
#include "bu/array.h"
#include "bu/signals.h"

namespace Bu
{
	class Stream;
	typedef Bu::Array<Bu::FString> StrArray;

	class CsvReader
	{
	public:
		typedef Bu::Signal1<Bu::FString, Bu::FString::iterator &> DecodeSignal;
		enum Style
		{
			styleExcel, ///< Excel style quotes around things that need em
			styleC		///< Escape things that need it C-style
		};

		CsvReader( Stream &sIn, Style eStyle=styleExcel );
		CsvReader( Stream &sIn, DecodeSignal sDecode );
		virtual ~CsvReader();

		StrArray readLine();

	private:
		Stream &sIn;
		DecodeSignal sDecode;

		static Bu::FString decodeExcel( Bu::FString::iterator &i );
		static Bu::FString decodeC( Bu::FString::iterator &i );
	};
};

#endif
