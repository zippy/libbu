/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef OSX_COMPATIBILITY__H
#define OSX_COMPATIBILITY__H

#ifdef __APPLE__

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression)            \
  (__extension__                                  \
    ({ long int __result;                         \
      do __result = (long int) (expression);      \
      while (__result == -1L && errno == EINTR);  \
      __result; }))
#endif

#include <sched.h>

#define pthread_yield() sched_yield()
#endif /* __APPLE__ */
#endif

