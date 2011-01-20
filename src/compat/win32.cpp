/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/compat/win32.h"

#ifdef WIN32

#define deffunc( name ) \
	Bu::Winsock2::FNDEF_DYN_ ##name Bu::Winsock2::_fnptr_ ##name = NULL

char Bu::Winsock2::scode[15];

deffunc( WSAStartup );
deffunc( WSACleanup );
deffunc( WSAGetLastError );
deffunc( inet_ntoa );
deffunc( inet_addr );
deffunc( select );
deffunc( socket );
deffunc( shutdown );
deffunc( ioctlsocket );
deffunc( htons );
deffunc( htonl );
deffunc( gethostbyname );
deffunc( freeaddrinfo );
deffunc( getaddrinfo );
deffunc( connect );
deffunc( getpeername );
deffunc( setsockopt );
deffunc( bind );
deffunc( listen );
deffunc( accept );	
deffunc( recv );
deffunc( send );
deffunc( __WSAFDIsSet );

// Safely get a function from the library
#define getfunc( name ) \
	Bu::Winsock2::_fnptr_ ##name = (FNDEF_DYN_ ##name) \
		GetProcAddress( Ws2_32, #name ); \
	if( Bu::Winsock2::_fnptr_ ##name == NULL ) { \
		throw Bu::ExceptionBase("Error loading function " #name " from dll.");\
	} (void)0

Bu::Winsock2::Winsock2()
{
	Ws2_32 = LoadLibrary(TEXT("WS2_32.DLL"));

	getfunc( WSAStartup );
	getfunc( WSACleanup );
	getfunc( WSAGetLastError );
	getfunc( inet_ntoa );
	getfunc( inet_addr );
	getfunc( select );
	getfunc( socket );
	getfunc( shutdown );
	getfunc( ioctlsocket );
	getfunc( htons );
	getfunc( htonl );
	getfunc( gethostbyname );
	getfunc( freeaddrinfo );
	getfunc( getaddrinfo );
	getfunc( connect );
	getfunc( getpeername );
	getfunc( setsockopt );
	getfunc( bind );
	getfunc( listen );
	getfunc( accept );	
	getfunc( recv );
	getfunc( send );
	getfunc( __WSAFDIsSet );

	Bu::Winsock2::WSAStartup( MAKEWORD(2, 2), &wsaData );
}

Bu::Winsock2::~Winsock2()
{
	Bu::Winsock2::WSACleanup();
	FreeLibrary( Ws2_32 );
}

char *Bu::Winsock2::gai_strerror( int iCode )
{
	sprintf( scode, "%d", Bu::Winsock2::WSAGetLastError() );
	return scode;
}

int Bu::Winsock2::WSAStartup( WORD a, LPWSADATA b ) {
	return (*Bu::Winsock2::_fnptr_WSAStartup)( a, b );
}
int Bu::Winsock2::WSACleanup( ) {
	return (*Bu::Winsock2::_fnptr_WSACleanup)();
}
int Bu::Winsock2::WSAGetLastError( ) {
	return (*Bu::Winsock2::_fnptr_WSAGetLastError)();
}
char * Bu::Winsock2::inet_ntoa( struct in_addr a ) {
	return (*Bu::Winsock2::_fnptr_inet_ntoa)( a );
}
unsigned long Bu::Winsock2::inet_addr( const char *s_in ) {
	return (*Bu::Winsock2::_fnptr_inet_addr)( s_in );
}
int Bu::Winsock2::select( int a, fd_set *b, fd_set *c, fd_set *d,
		const struct timeval *e ) {
	return (*Bu::Winsock2::_fnptr_select)( a, b, c, d, e );
}
SOCKET Bu::Winsock2::socket( int domain, int type, int protocol ) {
	return (*Bu::Winsock2::_fnptr_socket)( domain, type, protocol );
}
int Bu::Winsock2::shutdown( SOCKET s, int how ) {
	return (*Bu::Winsock2::_fnptr_shutdown)( s, how );
}
int Bu::Winsock2::ioctlsocket( SOCKET s, long cmd, u_long *argp ) {
	return (*Bu::Winsock2::_fnptr_ioctlsocket)( s, cmd, argp );
}
u_short Bu::Winsock2::htons( u_short in ) {
	return (*Bu::Winsock2::_fnptr_htons)( in );
}
u_long Bu::Winsock2::htonl( u_long in ) {
	return (*Bu::Winsock2::_fnptr_htonl)( in );
}
struct hostent * Bu::Winsock2::gethostbyname( const char *name ) {
	return (*Bu::Winsock2::_fnptr_gethostbyname)( name );
}
void Bu::Winsock2::freeaddrinfo( struct addrinfo *ai ) {
	return (*Bu::Winsock2::_fnptr_freeaddrinfo)( ai );
}
int Bu::Winsock2::getaddrinfo( const char *a, const char *b,
		const struct addrinfo *c, struct addrinfo **d ) {
	return (*Bu::Winsock2::_fnptr_getaddrinfo)( a, b, c, d );
}
int Bu::Winsock2::connect( SOCKET s, const struct sockaddr *a, int b ) {
	return (*Bu::Winsock2::_fnptr_connect)( s, a, b );
}
int Bu::Winsock2::getpeername( SOCKET s, struct sockaddr *a, int *b ) {
	return (*Bu::Winsock2::_fnptr_getpeername)( s, a, b);
}
int Bu::Winsock2::setsockopt( SOCKET s, int a, int b,
		const char *c, int d ) {
	return (*Bu::Winsock2::_fnptr_setsockopt)( s, a, b, c, d );
}
int Bu::Winsock2::bind( SOCKET s, const struct sockaddr *a, int b ) {
	return (*Bu::Winsock2::_fnptr_bind)( s, a, b );
}
int Bu::Winsock2::listen( SOCKET s, int backlog ) {
	return (*Bu::Winsock2::_fnptr_listen)( s, backlog );
}
SOCKET Bu::Winsock2::accept( SOCKET s, struct sockaddr *a, int *b ) {	
	return (*Bu::Winsock2::_fnptr_accept)( s, a, b );
}
int Bu::Winsock2::recv( SOCKET s, char *buf, int len, int flags ) {
	return (*Bu::Winsock2::_fnptr_recv)( s, buf, len, flags );
}
int Bu::Winsock2::send( SOCKET s, const char *buf, int len, int flags ) {
	return (*Bu::Winsock2::_fnptr_send)( s, buf, len, flags );
}
int Bu::Winsock2::__WSAFDIsSet( SOCKET s, fd_set *set ) {
	return (*Bu::Winsock2::_fnptr___WSAFDIsSet)( s, set );
}

Bu::String Bu::getLastWinError()
{
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR) &lpMsgBuf,
        0, NULL );

	Bu::String sRet( (char *)lpMsgBuf );

    LocalFree(lpMsgBuf);

	return sRet;
}

#endif

