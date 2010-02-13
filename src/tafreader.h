/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_READER_H
#define BU_TAF_READER_H

#include <stdint.h>
#include "bu/fstring.h"

namespace Bu
{
	class TafNode;
	class TafGroup;
	class TafProperty;
	class TafComment;
	class Stream;

	/**
	 *
	 *@ingroup Taf
	 */
	class TafReader
	{
	public:
		TafReader( Bu::Stream &sIn );
		virtual ~TafReader();

		Bu::TafGroup *readGroup();

	private:
		void groupContent( Bu::TafGroup *pNode );
		Bu::TafProperty *readProperty();
		Bu::TafComment *readComment( bool bEOL=false );
		void ws();
		bool isws();
		void next();
		Bu::FString readStr();
		void rawread( char *c );
		char c, la;
		Bu::Stream &sIn;
		int iLine, iCol;
	};
}

#endif
