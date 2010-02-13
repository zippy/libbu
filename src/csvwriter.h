/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CSV_WRITER_H
#define BU_CSV_WRITER_H

#include "bu/fstring.h"
#include "bu/array.h"
#include "bu/signals.h"

namespace Bu
{
	class Stream;
	typedef Bu::Array<Bu::FString> StrArray;

	class CsvWriter
	{
	public:
		typedef Bu::Signal1<Bu::FString, const Bu::FString &> EncodeSignal;
		enum Style
		{
			styleExcel, ///< Excel style quotes around things that need em
			styleC		///< Escape things that need it C-style
		};

		CsvWriter( Stream &sOut, Style eStyle=styleExcel );
		CsvWriter( Stream &sOut, EncodeSignal sEncode );
		virtual ~CsvWriter();

	private:
		Stream &sOut;
		EncodeSignal sEncode;

		static Bu::FString encodeExcel( const Bu::FString &sIn );
		static Bu::FString encodeC( const Bu::FString &sIn );
	};
};

#endif
