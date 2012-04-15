/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CRYPT_H
#define BU_CRYPT_H

#include "bu/string.h"

namespace Bu
{
	String cryptPass( const Bu::String &sPass, const Bu::String &sSalt );
	String cryptPass( const Bu::String &sPass );
};

#endif
