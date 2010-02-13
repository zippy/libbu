/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CRYPT_H
#define BU_CRYPT_H

#include "bu/fstring.h"

namespace Bu
{
	FString cryptPass( const Bu::FString &sPass, const Bu::FString &sSalt );
	FString cryptPass( const Bu::FString &sPass );
};

#endif
