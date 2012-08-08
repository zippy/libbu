/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef WIN32_COMPATIBILITY__H
#define WIN32_COMPATIBILITY__H

#ifdef WIN32

#ifdef __cplusplus
extern "C" 
{
#define FD_SETSIZE (4096*2)
#include <Winsock2.h>
#include <ws2tcpip.h>
}
#endif

#include "bu/string.h"
#include "bu/singleton.h"

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression)            \
  (__extension__                                  \
    ({ long int __result;                         \
      do __result = (long int) (expression);      \
      while (__result == -1L && errno == EINTR);  \
      __result; }))
#endif

#define decltype( ret, name, ... ) \
	typedef ret (__cdecl *FNDEF_DYN_ ##name)( __VA_ARGS__ ); \
	static FNDEF_DYN_ ##name _fnptr_ ##name; \
	static ret name( __VA_ARGS__ )

__extension__ typedef int socklen_t;

#ifdef gai_strerror
#undef gai_strerror
#endif

namespace Bu
{
	class Winsock2 : public Bu::Singleton<Winsock2>
	{
	friend class Bu::Singleton<Winsock2>;
	private:
		Winsock2();
		virtual ~Winsock2();

		WSADATA wsaData;
		HINSTANCE Ws2_32;

	public:
		// decltype( return type, function name<, optional parameters> )
		decltype( int, WSAStartup, WORD, LPWSADATA );
		decltype( int, WSACleanup );
		decltype( int, WSAGetLastError );
		decltype( char *, inet_ntoa, struct in_addr );
		decltype( unsigned long, inet_addr, const char *s_in );
		decltype( int, select, int nfds, fd_set *readfds, fd_set *writefds, 
				fd_set *exceptfds, const struct timeval *timeout );
		decltype( SOCKET, socket, int domain, int type, int protocol );
		decltype( int, shutdown, SOCKET s, int how );
		decltype( int, ioctlsocket, SOCKET s, long cmd, u_long *argp );
		decltype( u_short, htons, u_short in );
		decltype( u_long, htonl, u_long in );
		decltype( struct hostent *, gethostbyname, const char *name );
		decltype( void, freeaddrinfo, struct addrinfo *ai );
		decltype( int, getaddrinfo, const char *nodename, const char *servname,
				const struct addrinfo *hints, struct addrinfo **res );
		decltype( int, connect, SOCKET s, const struct sockaddr *serv_addr,
				int addrlen );
		decltype( int, getpeername, SOCKET s, struct sockaddr *name,
				int *namelen );
		decltype( int, setsockopt, SOCKET s, int level, int optname,
				const char *optval, int optlen );
		decltype( int, bind, SOCKET s, const struct sockaddr *my_addr,
				int addrlen );
		decltype( int, listen, SOCKET s, int backlog );
		decltype( SOCKET, accept, SOCKET s, struct sockaddr *addr,
				int *addrlen);	
		decltype( int, recv, SOCKET s, char *buf, int len, int flags );
		decltype( int, send, SOCKET s, const char *buf, int len, int flags );
		decltype( int, __WSAFDIsSet, SOCKET s, fd_set *set );
	
		static char scode[15];
		static char *gai_strerror( int iCode );
	};

	Bu::String getLastWinError();
};

#ifdef FD_ISSET
#undef FD_ISSET
#endif

#define bu_WSAStartup (*Bu::Winsock2::WSAStartup)
#define bu_WSACleanup (*Bu::Winsock2::WSACleanup)
#define bu_WSAGetLastError (*Bu::Winsock2::WSAGetLastError)
#define bu_inet_ntoa (*Bu::Winsock2::inet_ntoa)
#define bu_inet_addr (*Bu::Winsock2::inet_addr)
#define bu_select (*Bu::Winsock2::select)
#define bu_socket (*Bu::Winsock2::socket)
#define bu_shutdown (*Bu::Winsock2::shutdown)
#define bu_ioctlsocket (*Bu::Winsock2::ioctlsocket)
#define bu_htons (*Bu::Winsock2::htons)
#define bu_htonl (*Bu::Winsock2::htonl)
#define bu_gethostbyname (*Bu::Winsock2::gethostbyname)
#define bu_freeaddrinfo (*Bu::Winsock2::freeaddrinfo)
#define bu_getaddrinfo (*Bu::Winsock2::getaddrinfo)
#define bu_connect (*Bu::Winsock2::connect)
#define bu_getpeername (*Bu::Winsock2::getpeername)
#define bu_setsockopt (*Bu::Winsock2::setsockopt)
#define bu_bind (*Bu::Winsock2::bind)
#define bu_listen (*Bu::Winsock2::listen)
#define bu_accept (*Bu::Winsock2::accept)	
#define bu_recv (*Bu::Winsock2::recv)
#define bu_send (*Bu::Winsock2::send)
#define bu___WSAFDIsSet (*Bu::Winsock2::__WSAFDIsSet)

#define FD_ISSET (*Bu::Winsock2::__WSAFDIsSet)
#define bu_gai_strerror Bu::Winsock2::gai_strerror

#undef decltype

#undef USE_64BIT_IO

// Windows has no autoconfig, and no endian.h, but only works on little endian
// as far as I know.
#define LITTLE_ENDIAN 0
#define BIG_ENDIAN 1
#define BYTE_ORDER 0

#endif /* WIN32 */
#endif

