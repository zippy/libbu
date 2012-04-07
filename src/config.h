/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CONFIG_H
#define BU_CONFIG_H

// Use 64 bit IO functions where applicable (i.e. large file support)
#define USE_64BIT_IO

#include "bu/compat/win32.h"
#include "bu/compat/osx.h"
#include "bu/compat/linux.h"

#include "bu/extratypes.h"

#ifndef WIN32
#include "bu/autoconfig.h"
#else
#define	__LITTLE_ENDIAN	1234
#define	__BIG_ENDIAN	4321
#define	__PDP_ENDIAN	3412
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif


#endif
