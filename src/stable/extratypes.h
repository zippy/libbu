/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef EXTRA_TYPES_H
#define EXTRA_TYPES_H

#include "bu/config.h"

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

namespace Bu
{
#ifdef USE_64BIT_IO
    typedef int64_t     size;
    typedef uint64_t    usize;
#else
    typedef int32_t     size;
    typedef uint32_t    usize;
#endif
};

#endif
