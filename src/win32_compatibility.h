/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef WIN32_COMPATIBILITY__H
#define WIN32_COMPATIBILITY__H

#ifdef WIN32
 #define DYNLOAD DynamicWinsock2::
#else
 #define DYNLOAD 
#endif

#ifdef WIN32

#include <Winsock2.h>
#include "fstring.h"

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression)            \
  (__extension__                                  \
    ({ long int __result;                         \
      do __result = (long int) (expression);      \
      while (__result == -1L && errno == EINTR);  \
      __result; }))
#endif

__extension__ typedef int socklen_t;

namespace DynamicWinsock2
{
	void inet_ntoa( Bu::FString &out, struct in_addr addr_in );
	unsigned long inet_addr( const char *s_in );
	int select(int nfds, fd_set *readfds, fd_set *writefds, 
			fd_set *exceptfds, const struct timeval *timeout);
	SOCKET socket(int domain, int type, int protocol);
	int ioctlsocket(SOCKET s, long cmd, u_long *argp);
	u_short htons(u_short in);
	u_long htonl(u_long in);
	struct hostent *gethostbyname(const char *name);
	int connect(SOCKET s, const struct sockaddr *serv_addr, int addrlen);
	int getpeername(SOCKET s, struct sockaddr *name, int *namelen);	
	int setsockopt(SOCKET s, int level, int optname, 
			const char *optval, int optlen);
	int bind(SOCKET s, const struct sockaddr *my_addr, int addrlen);
	int listen(SOCKET s, int backlog);	
	SOCKET accept(SOCKET s, struct sockaddr *addr, int *addrlen);	
	int recv( SOCKET s, char *buf, int len, int flags );
	int send( SOCKET s, const char *buf, int len, int flags );
	int DYN_FD_ISSET(SOCKET s, fd_set *set);
};

#endif /* WIN32 */
#endif

