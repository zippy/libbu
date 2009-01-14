#include "win32_compatibility.h"

#ifdef WIN32

typedef int (__cdecl *FNDEF_DYN_WSAStartup)(WORD,LPWSADATA);
int DynamicWinsock2::WSAStartup(
	WORD wVersionRequested,LPWSADATA lpWSAData)
{
	int out=0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("WS2_32.DLL"));
	printf("ws2_32 dll: %08x\n", (int) Ws2_32);
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_WSAStartup fn = (FNDEF_DYN_WSAStartup)
			GetProcAddress( Ws2_32, "WSAStartup" );
		printf("WSAStartup function pointer: %08x\n", (int)fn);
		if( fn != NULL )
			out = (fn)(wVersionRequested,lpWSAData);
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_WSACleanup)();
int DynamicWinsock2::WSACleanup()
{
	int out=0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("WS2_32.DLL"));
	printf("ws2_32 dll: %08x\n", (int) Ws2_32);
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_WSACleanup fn = (FNDEF_DYN_WSACleanup)
			GetProcAddress( Ws2_32, "WSACleanup" );
		printf("WSACleanup function pointer: %08x\n", (int)fn);
		if( fn != NULL )
			out = (fn)();
	}
	return out;
}

//typedef char * (__cdecl *FNDEF_DYN_gai_strerrorA)( int ecode );
typedef int (__cdecl *FNDEF_DYN_WSAGetLastError)(void);
int DynamicWinsock2::WSAGetLastError()
{
	int out=0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("WS2_32.DLL"));
	printf("ws2_32 dll: %08x\n", (int) Ws2_32);
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_WSAGetLastError fn = (FNDEF_DYN_WSAGetLastError)
			GetProcAddress( Ws2_32, "WSAGetLastError" );
		printf("WSAGetLastError function pointer: %08x\n", (int)fn);
		if( fn != NULL )
			out = (fn)();
	}
	return out;
}

typedef char * (__cdecl *FNDEF_DYN_inet_ntoa)( struct in_addr );
void DynamicWinsock2::inet_ntoa( Bu::FString &out, struct in_addr addr_in )
{
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_inet_ntoa fn = (FNDEF_DYN_inet_ntoa)
			GetProcAddress( Ws2_32, "inet_ntoa" );
		if( fn != NULL )
			out = (fn)( addr_in );

		//We will let windows clean up our dll imports on exit
		//FreeLibrary( Ws2_32 );
	}
}

typedef unsigned long (__cdecl *FNDEF_DYN_inet_addr)( const char * );
unsigned long DynamicWinsock2::inet_addr( const char *s_in )
{
	unsigned long out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_inet_addr fn = (FNDEF_DYN_inet_addr)
			GetProcAddress( Ws2_32, "inet_addr" );
		if( fn != NULL )
			out = (fn)( s_in );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_select)(
		int nfds,fd_set*,fd_set*,fd_set*,const struct timeval*);
int DynamicWinsock2::select(int nfds, fd_set *readfds, fd_set *writefds, 
			fd_set *exceptfds, const struct timeval *timeout)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_select fn = (FNDEF_DYN_select)
			GetProcAddress( Ws2_32, "select" );
		if( fn != NULL )
			out = (fn)( nfds, readfds, writefds, exceptfds, timeout );
	}
	return out;
}

typedef SOCKET (__cdecl *FNDEF_DYN_socket)(int,int,int);
SOCKET DynamicWinsock2::socket(int domain, int type, int protocol)
{
	printf("in win32::socket\n");
	SOCKET out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	printf("ws2_32 dll: %08x\n", (int) Ws2_32);
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_socket fn = (FNDEF_DYN_socket)
			GetProcAddress( Ws2_32, "socket" );
		printf("socket function pointer: %08x\n", (int)fn);
		if( fn != NULL )
			out = (fn)( domain, type, protocol );
	}
	printf("win32::socket complete.\n");
	return out;
}

typedef int (__cdecl *FNDEF_DYN_ioctlsocket)(SOCKET,long,u_long *);
int DynamicWinsock2::ioctlsocket(SOCKET s, long cmd, u_long *argp)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_ioctlsocket fn = (FNDEF_DYN_ioctlsocket)
			GetProcAddress( Ws2_32, "ioctlsocket" );
		if( fn != NULL )
			out = (fn)( s, cmd, argp );
	}
	return out;
}

typedef u_short (__cdecl *FNDEF_DYN_htons)(u_short);
u_short DynamicWinsock2::htons(u_short in)
{
	u_short out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_htons fn = (FNDEF_DYN_htons)
			GetProcAddress( Ws2_32, "htons" );
		if( fn != NULL )
			out = (fn)( in );
	}
	return out;
}

typedef u_long (__cdecl *FNDEF_DYN_htonl)(u_long);
u_long DynamicWinsock2::htonl(u_long in)
{
	u_long out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_htonl fn = (FNDEF_DYN_htonl)
			GetProcAddress( Ws2_32, "htonl" );
		if( fn != NULL )
			out = (fn)( in );
	}
	return out;
}

typedef struct hostent * (__cdecl *FNDEF_DYN_gethostbyname)(const char *);
struct hostent *DynamicWinsock2::gethostbyname(const char *name)
{
	hostent *out = NULL;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_gethostbyname fn = (FNDEF_DYN_gethostbyname)
			GetProcAddress( Ws2_32, "gethostbyname" );
		if( fn != NULL )
			out = (fn)( name );
	}
	return out;
}

typedef void (__cdecl *FNDEF_DYN_freeaddrinfo)(struct addrinfo *);
void DynamicWinsock2::freeaddrinfo(struct addrinfo *ai)
{
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_freeaddrinfo fn = (FNDEF_DYN_freeaddrinfo)
			GetProcAddress( Ws2_32, "freeaddrinfo" );
		if( fn != NULL )
			(fn)( ai );
	}
}

typedef int (__cdecl *FNDEF_DYN_getaddrinfo)(
		const char*,const char*,const struct addrinfo*,struct addrinfo**);
int DynamicWinsock2::getaddrinfo(
		const char *nodename, const char *servname,
		const struct addrinfo *hints, struct addrinfo **res )
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_getaddrinfo fn = (FNDEF_DYN_getaddrinfo)
			GetProcAddress( Ws2_32, "getaddrinfo" );
		if( fn != NULL )
			out = (fn)( nodename, servname, hints, res );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_connect)(SOCKET,const struct sockaddr*,int);
int DynamicWinsock2::connect(
		SOCKET s, const struct sockaddr *serv_addr, int addrlen)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_connect fn = (FNDEF_DYN_connect)
			GetProcAddress( Ws2_32, "connect" );
		if( fn != NULL )
			out = (fn)( s, serv_addr, addrlen );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_getpeername)(SOCKET,struct sockaddr*,int*);
int DynamicWinsock2::getpeername(SOCKET s, struct sockaddr *name, int *namelen)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_getpeername fn = (FNDEF_DYN_getpeername)
			GetProcAddress( Ws2_32, "getpeername" );
		if( fn != NULL )
			out = (fn)( s, name, namelen );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_setsockopt)(SOCKET,int,int,const char*,int);
int DynamicWinsock2::setsockopt(SOCKET s, int level, int optname, 
		const char *optval, int optlen)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_setsockopt fn = (FNDEF_DYN_setsockopt)
			GetProcAddress( Ws2_32, "setsockopt" );
		if( fn != NULL )
			out = (fn)( s, level, optname, optval, optlen );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_bind)(SOCKET,const struct sockaddr*,int);
int DynamicWinsock2::bind(SOCKET s, const struct sockaddr *my_addr, int addrlen)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_bind fn = (FNDEF_DYN_bind)
			GetProcAddress( Ws2_32, "bind" );
		if( fn != NULL )
			out = (fn)( s, my_addr, addrlen );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_listen)(SOCKET,int);
int DynamicWinsock2::listen(SOCKET s, int backlog)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_listen fn = (FNDEF_DYN_listen)
			GetProcAddress( Ws2_32, "listen" );
		if( fn != NULL )
			out = (fn)( s, backlog );
	}
	return out;
}

typedef SOCKET (__cdecl *FNDEF_DYN_accept)(SOCKET,struct sockaddr*,int*);
SOCKET DynamicWinsock2::accept(SOCKET s, struct sockaddr *addr, int *addrlen)
{
	SOCKET out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_accept fn = (FNDEF_DYN_accept)
			GetProcAddress( Ws2_32, "accept" );
		if( fn != NULL )
			out = (fn)( s, addr, addrlen );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_recv)(SOCKET,char*,int,int);
int DynamicWinsock2::recv( SOCKET s, char *buf, int len, int flags )
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_recv fn = (FNDEF_DYN_recv)
			GetProcAddress( Ws2_32, "recv" );
		if( fn != NULL )
			out = (fn)( s, buf, len, flags );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN_send)(SOCKET,const char*,int,int);
int DynamicWinsock2::send( SOCKET s, const char *buf, int len, int flags )
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN_send fn = (FNDEF_DYN_send)
			GetProcAddress( Ws2_32, "send" );
		if( fn != NULL )
			out = (fn)( s, buf, len, flags );
	}
	return out;
}

typedef int (__cdecl *FNDEF_DYN__WSAFDIsSet)(SOCKET,fd_set*);
int DynamicWinsock2::DYN_FD_ISSET(SOCKET s, fd_set *set)
{
	int out = 0;
	HINSTANCE Ws2_32 = LoadLibrary(TEXT("Ws2_32"));
	if( Ws2_32 != NULL )
	{
		FNDEF_DYN__WSAFDIsSet fn = (FNDEF_DYN__WSAFDIsSet)
			GetProcAddress( Ws2_32, "__WSAFDIsSet" );
		if( fn != NULL )
			out = (fn)( s, set );
	}
	return out;
}

#endif

