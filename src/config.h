/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
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
# include "bu/autoconfig.h"
#else
# define    __LITTLE_ENDIAN 1234
# define    __BIG_ENDIAN    4321
# define    __PDP_ENDIAN    3412
# define __BYTE_ORDER __LITTLE_ENDIAN

# ifndef LITTLE_ENDIAN
#  define LITTLE_ENDIAN __LITTLE_ENDIAN
# endif
# ifndef BIG_ENDIAN
#  define BIG_ENDIAN __BIG_ENDIAN
# endif
# ifndef PDP_ENDIAN
#  define PDP_ENDIAN __PDP_ENDIAN
# endif
# ifndef BYTE_ORDER
#  define BYTE_ORDER __BYTE_ORDER
# endif
#endif

// At this point the system endian.h should be included, but if it's missing
// an endian conversion macro, lets fix that now...

#define bu_byte_swap16(x) (((x&0xffu)<<8)|((x&0xff00u)>>8))
#define bu_byte_swap32(x) (((x&0xfful)<<24)|((x&0xff00ul)<<8)|((x&0xff0000ul)>>8)|((x&0xff000000ul)>>24))
#define bu_byte_swap64(x) (((x&0xffull)<<56)|((x&0xff00ull)<<40)|((x&0xff0000ull)<<24)|((x&0xff000000ull)<<8)|((x&0xff00000000000000ull)>>56)|((x&0xff000000000000ull)>>40)|((x&0xff0000000000ull)>>24)|((x&0xff00000000ull)>>8))

#if BYTE_ORDER == LITTLE_ENDIAN
# ifndef  htobe16
#  define htobe16(x) bu_byte_swap16(x)
# endif
# ifndef  htole16
#  define htole16(x) (x)
# endif
# ifndef  be16toh
#  define be16toh(x) bu_byte_swap16(x)
# endif
# ifndef  le16toh
#  define le16toh(x) (x)
# endif

# ifndef  htobe32
#  define htobe32(x) bu_byte_swap32(x)
# endif
# ifndef  htole32
#  define htole32(x) (x)
# endif
# ifndef  be32toh
#  define be32toh(x) bu_byte_swap32(x)
# endif
# ifndef  le32toh
#  define le32toh(x) (x)
# endif

# ifndef  htobe64
#  define htobe64(x) bu_byte_swap64(x)
# endif
# ifndef  htole64
#  define htole64(x) (x)
# endif
# ifndef  be64toh
#  define be64toh(x) bu_byte_swap64(x)
# endif
# ifndef  le64toh
#  define le64toh(x) (x)
# endif
#elif BYTE_ORDER == BIG_ENDIAN
# ifndef  htole16
#  define htole16(x) bu_byte_swap16(x)
# endif
# ifndef  htobe16
#  define htobe16(x) (x)
# endif
# ifndef  le16toh
#  define le16toh(x) bu_byte_swap16(x)
# endif
# ifndef  be16toh
#  define be16toh(x) (x)
# endif

# ifndef  htole32
#  define htole32(x) bu_byte_swap32(x)
# endif
# ifndef  htobe32
#  define htobe32(x) (x)
# endif
# ifndef  le32toh
#  define le32toh(x) bu_byte_swap32(x)
# endif
# ifndef  be32toh
#  define be32toh(x) (x)
# endif

# ifndef  htole64
#  define htole64(x) bu_byte_swap64(x)
# endif
# ifndef  htobe64
#  define htobe64(x) (x)
# endif
# ifndef  le64toh
#  define le64toh(x) bu_byte_swap64(x)
# endif
# ifndef  be64toh
#  define be64toh(x) (x)
# endif
#else
#error Unhandled endianness detected
#endif

#endif
