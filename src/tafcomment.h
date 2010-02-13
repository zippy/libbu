/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_COMMENT_H
#define BU_TAF_COMMENT_H

#include <stdint.h>
#include "bu/tafnode.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Taf
	 */
	class TafComment : public TafNode
	{
	public:
		TafComment( const Bu::FString &sText, bool bEOL=false );
		virtual ~TafComment();

		const Bu::FString &getText() const;
		bool isEOLStyle() const;

	private:
		Bu::FString sText;
		bool bEOL;
	};
}

#endif
