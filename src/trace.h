/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_TRACE_H
#define BU_TRACE_H

#ifdef BU_TRACE
# define TRACE() printf("trace: %s\n", __PRETTY_FUNCTION__ )
#else
# define TRACE() {}
#endif

#endif
