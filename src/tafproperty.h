/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TAF_PROPERTY_H
#define BU_TAF_PROPERTY_H

#include <stdint.h>
#include "bu/tafnode.h"

namespace Bu
{
	/**
	 *
	 *@ingroup Taf
	 */
	class TafProperty : public TafNode
	{
	public:
		TafProperty( const Bu::FString &sName, const Bu::FString &sValue );
		virtual ~TafProperty();

		const Bu::FString &getName() const;
		const Bu::FString &getValue() const;

	private:
		Bu::FString sName;
		Bu::FString sValue;
	};
}

#endif
