/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifdef __linux__

#define bu_inet_ntoa inet_ntoa
#define bu_inet_addr inet_addr
#define bu_select select
#define bu_socket socket
#define bu_shutdown shutdown
#define bu_htons htons
#define bu_htonl htonl
#define bu_gethostbyname gethostbyname
#define bu_freeaddrinfo freeaddrinfo
#define bu_getaddrinfo getaddrinfo
#define bu_connect connect
#define bu_getpeername getpeername
#define bu_setsockopt setsockopt
#define bu_bind bind
#define bu_listen listen
#define bu_accept accept
#define bu_send send
#define bu_recv recv

#define bu_gai_strerror gai_strerror

#endif
