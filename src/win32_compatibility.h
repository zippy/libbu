/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef WIN32_COMPATIBILITY__H
#define WIN32_COMPATIBILITY__H

#ifdef WIN32

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression)            \
  (__extension__                                  \
    ({ long int __result;                         \
      do __result = (long int) (expression);      \
      while (__result == -1L && errno == EINTR);  \
      __result; }))
#endif

//__extension__ typedef unsigned int socklen_t;
__extension__ typedef int socklen_t;

#endif /* WIN32 */
#endif

