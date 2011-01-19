#ifndef EXTRA_TYPES_H
#define EXTRA_TYPES_H

#include "bu/config.h"

namespace Bu
{
#ifdef USE_64BIT_IO
	typedef int64_t		size;
	typedef uint64_t	usize;
#else
	typedef int32_t		size;
	typedef uint32_t	usize;
#endif
};

#endif
