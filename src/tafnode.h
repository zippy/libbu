/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_NODE_H
#define BU_TAF_NODE_H

#include <stdint.h>
#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDecl( TafException );
	/**
	 *
	 *@ingroup Taf
	 */
	class TafNode
	{
	public:
		enum NodeType
		{
			typeGroup,
			typeProperty,
			typeComment
		};

	public:
		TafNode( NodeType eType );
		virtual ~TafNode();

		NodeType getType() const;

	private:
		NodeType eType;
	};
}

#endif
