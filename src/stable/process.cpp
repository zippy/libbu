/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/process.h"
#include <unistd.h>
#include <stdarg.h>

#ifndef WIN32
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>
#include <errno.h>
#else
#include <windows.h> 
#include <tchar.h>
#include <wchar.h>
#endif


#include "bu/config.h"

Bu::Process::ProcData::ProcData() :
#ifdef WIN32
	hStdIn( NULL ),
	hStdOut( NULL ),
	hStdErr( NULL )
#else
	iStdIn( -1 ),
	iStdOut( -1 ),
	iStdErr( -1 )
#endif
{
}


Bu::Process::Process( Flags eFlags, const char *sName, char *const argv[] ) :
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true )
{
	gexec( eFlags, sName, argv );
}

Bu::Process::Process( Flags eFlags, const char *sName, const char *argv, ...) :
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true )
{
	int iCnt = 0;
	va_list ap;
	va_start( ap, argv );
	for(; va_arg( ap, const char *); iCnt++ ) { }
	va_end( ap );

	char const **list = new char const *[iCnt+2];
	va_start( ap, argv );
	list[0] = argv;
	for( int j = 1; j <= iCnt; j++ )
	{
		list[j] = va_arg( ap, const char *);
	}
	list[iCnt+1] = NULL;
	va_end( ap );

	gexec( eFlags, sName, (char *const *)list );
	delete[] list;
}

Bu::Process::Process( Flags eFlags, const Bu::Process::Options &opt, const char *sName, char *const argv[] ) :
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true ),
	opt( opt )
{
	gexec( eFlags, sName, argv );
}

Bu::Process::Process( Flags eFlags, const Bu::Process::Options &opt, const char *sName, const char *argv, ...) :
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true ),
	opt( opt )
{
	int iCnt = 0;
	va_list ap;
	va_start( ap, argv );
	for(; va_arg( ap, const char *); iCnt++ ) { }
	va_end( ap );

	char const **list = new char const *[iCnt+2];
	va_start( ap, argv );
	list[0] = argv;
	for( int j = 1; j <= iCnt; j++ )
	{
		list[j] = va_arg( ap, const char *);
	}
	list[iCnt+1] = NULL;
	va_end( ap );

	gexec( eFlags, sName, (char *const *)list );
	delete[] list;
}

Bu::Process::~Process()
{
	close();
}

void Bu::Process::wait()
{
	close();
}

void Bu::Process::gexec( Flags eFlags, const char *sName, char *const argv[] )
{
#ifdef WIN32
	bStdErrEos = true;
	SECURITY_ATTRIBUTES saAttr;
	HANDLE hChildStd_IN_Rd;
	HANDLE hChildStd_IN_Wr;
	HANDLE hChildStd_OUT_Rd;
	HANDLE hChildStd_OUT_Wr;
	PROCESS_INFORMATION piProcInfo;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if ( ! CreatePipe(
				&hChildStd_OUT_Rd,
				&hChildStd_OUT_Wr,
				&saAttr, 0) ) 
		throw "StdoutRd CreatePipe"; 

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if ( ! SetHandleInformation(
				hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
		throw "Stdout SetHandleInformation"; 

	// Create a pipe for the child process's STDIN. 
	if (! CreatePipe(
				&hChildStd_IN_Rd,
				&hChildStd_IN_Wr,
				&saAttr, 0)) 
		throw "Stdin CreatePipe"; 

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if ( ! SetHandleInformation(
				hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
		throw "Stdin SetHandleInformation"; 

	//TCHAR szCmdline[] = TEXT( sName );
	//PROCESS_INFORMATION pd.piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE; 

	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdError = hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = hChildStd_OUT_Wr;
	siStartInfo.hStdInput = hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	Bu::String sCmd;
	for( char *const *arg = argv; *arg; arg++ )
	{
		if( arg != argv )
			sCmd += " ";
		sCmd += *arg;
	}

	// Create the child process. 
	bSuccess = CreateProcessA(
		sName, //sName, 			//module name
		sCmd.getStr(), //(TCHAR *) sName, //szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		CREATE_NO_WINDOW,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo );  // receives PROCESS_INFORMATION 

	// If an error occurs, exit the application. 
	if ( ! bSuccess ) 
	{
		throw Bu::ExceptionBase("Error spawning child process.");
		return;
	}
	else 
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 
		
		//CloseHandle(pData->pd.piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		pd.hProcess = piProcInfo.hProcess;

		// Close the ends we can't use
		CloseHandle( hChildStd_OUT_Wr );
		CloseHandle( hChildStd_IN_Rd );
		pd.hStdIn = hChildStd_IN_Wr;
		pd.hStdOut = hChildStd_OUT_Rd;
	}
#else
	int iaStdIn[2];
	int iaStdOut[2];
	int iaStdErr[2];
	pipe( iaStdIn );
	if( eFlags & StdOut )
	{
		pipe( iaStdOut );
		pd.iStdOut = iaStdOut[0];
		bStdOutEos = false;
	}
	if( eFlags & StdErr )
	{
		pipe( iaStdErr );
		pd.iStdErr = iaStdErr[0];
		bStdErrEos = false;
	}

	pd.iStdIn = iaStdIn[1];

//	fcntl( pd.iStdOut, F_SETFL, fcntl( pd.iStdOut, F_GETFL, 0 )|O_NONBLOCK );

	pd.iPid = fork();
	if( pd.iPid == 0 )
	{
		::close( iaStdIn[1] );
		dup2( iaStdIn[0], 0 );
		if( eFlags & StdOut )
		{
			::close( iaStdOut[0] );
			dup2( iaStdOut[1], 1 );
		}
		if( eFlags & StdErr )
		{
			::close( iaStdErr[0] );
			dup2( iaStdErr[1], 2 );
		}
		if( (opt.eFlags&Options::SetGid) )
		{
			setgid( opt.iGid );
		}
		if( (opt.eFlags&Options::SetUid) )
		{
			setuid( opt.iUid );
		}
		execvp( sName, argv );
		throw Bu::ExceptionBase("Hey, execvp failed!");
	}
	::close( iaStdIn[0] );
	if( eFlags & StdOut )
		::close( iaStdOut[1] );
	if( eFlags & StdErr )
		::close( iaStdErr[1] );
#endif
}

void Bu::Process::close()
{
#ifdef WIN32
	if( pd.hStdIn )
	{
		CloseHandle( pd.hStdIn );
		CloseHandle( pd.hStdOut );

		pd.hStdIn = pd.hStdOut = pd.hStdErr = NULL;

		if( !TerminateProcess(pd.hProcess, 1) )
		{
			throw Bu::ExceptionBase("Error closing process.");
		}
		
		GetExitCodeProcess( pd.hProcess, (PDWORD)&iProcStatus );

		CloseHandle( pd.hProcess );
		pd.hProcess = NULL;
	}
#else
	if( pd.iPid )
	{
		if( pd.iStdIn > -1 )
			::close( pd.iStdIn );
		if( pd.iStdOut > -1 )
			::close( pd.iStdOut );
		if( pd.iStdErr > -1 )
			::close( pd.iStdErr );
		waitpid( pd.iPid, &iProcStatus, 0 );
		pd.iPid = 0;
	}
#endif
}

void Bu::Process::closeStdIn()
{
#ifdef WIN32
	CloseHandle( pd.hStdIn );
	pd.hStdIn = NULL;
#else
	::close( pd.iStdIn );
	pd.iStdIn = -1;
#endif
}

void Bu::Process::closeStdOut()
{
#ifdef WIN32
	CloseHandle( pd.hStdOut );
	pd.hStdOut = NULL;
#else
	::close( pd.iStdOut );
	pd.iStdOut = -1;
#endif
}

Bu::size Bu::Process::read( void *pBuf, Bu::size nBytes )
{
#ifdef WIN32
	DWORD dwRead;
	DWORD lExitCode;
	BOOL bSuccess = FALSE;
	DWORD dwLen = (DWORD) nBytes;
	bSuccess = ReadFile(
			pd.hStdOut, (char *) pBuf,
			dwLen, &dwRead, NULL);
//	if( dwRead < dwLen )
	{
		bSuccess = GetExitCodeProcess( pd.hProcess, &lExitCode );
		if( lExitCode != STILL_ACTIVE )
		{
			bStdOutEos = true;
		}
	}
	return (int32_t) dwRead;
#else
	if( bStdOutEos )
		return 0;
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( pd.iStdOut, &rfds );
	struct timeval tv = {0, 0};
	if( ::bu_select( pd.iStdOut+1, &rfds, NULL, NULL, &tv ) < 0 )
		throw Bu::ExceptionBase( strerror( errno ) );
	if( FD_ISSET( pd.iStdOut, &rfds ) || bBlocking )
	{
		Bu::size nRead = TEMP_FAILURE_RETRY( ::read( pd.iStdOut, pBuf, nBytes ) );
		if( nRead == 0 )
		{
			bStdOutEos = true;
			checkClose();
			return 0;
		}
		if( nRead < 0 )
		{
			if( errno == EAGAIN )
				return 0;
			throw Bu::ExceptionBase( strerror( errno ) );
		}
		return nRead;
	}
	return 0;
#endif
}

Bu::size Bu::Process::readErr( void *pBuf, Bu::size nBytes )
{
#ifdef WIN32
	if( !pd.hStdErr )
		return 0;
	DWORD dwRead;
	BOOL bSuccess = FALSE;
	DWORD dwLen = (DWORD) nBytes;
	bSuccess = ReadFile(
			pd.hStdErr, (char *) pBuf,
			dwLen, &dwRead, NULL);
	return (int32_t) dwRead;
#else
	if( bStdErrEos )
		return 0;
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( pd.iStdErr, &rfds );
	struct timeval tv = {0, 0};
	if( ::bu_select( pd.iStdErr+1, &rfds, NULL, NULL, &tv ) < 0 )
		throw Bu::ExceptionBase( strerror( errno ) );
	if( FD_ISSET( pd.iStdErr, &rfds ) || bBlocking )
	{
		Bu::size nRead = TEMP_FAILURE_RETRY( ::read( pd.iStdErr, pBuf, nBytes ) );
		if( nRead == 0 )
		{
			bStdErrEos = true;
			checkClose();
			return 0;
		}
		if( nRead < 0 )
		{
			if( errno == EAGAIN )
				return 0;
			throw Bu::ExceptionBase( strerror( errno ) );
		}
		return nRead;
	}
	return 0;
#endif
}

Bu::size Bu::Process::write( const void *pBuf, Bu::size nBytes )
{
#ifdef WIN32
	DWORD dwWritten;
	BOOL bSuccess = FALSE;
	DWORD dwLen = (DWORD) nBytes;
	bSuccess = WriteFile( 
			pd.hStdIn, (const char *) pBuf,
			dwLen, &dwWritten, NULL );
	FlushFileBuffers( pd.hStdIn );
	return (int32_t) dwWritten;
#else
	return TEMP_FAILURE_RETRY( ::write( pd.iStdIn, pBuf, nBytes ) );
#endif
}

Bu::size Bu::Process::tell()
{
	return 0;
}

void Bu::Process::seek( Bu::size )
{
}

void Bu::Process::setPos( Bu::size )
{
}

void Bu::Process::setPosEnd( Bu::size )
{
}

bool Bu::Process::isEos()
{
#ifdef WIN32
	return (pd.hProcess == NULL);
#else
	return (pd.iPid == 0);
#endif
}

bool Bu::Process::isOpen()
{
	return !isEos();
}

void Bu::Process::flush()
{
}

bool Bu::Process::canRead()
{
	return true;
}

bool Bu::Process::canWrite()
{
	return true;
}

bool Bu::Process::isReadable()
{
	return true;
}

bool Bu::Process::isWritable()
{
	return true;
}

bool Bu::Process::isSeekable()
{
	return false;
}

bool Bu::Process::isBlocking()
{
	return true;
}

void Bu::Process::setBlocking( bool bBlocking )
{
#ifdef WIN32
#else
	if( bBlocking )
	{
		if( !bStdOutEos )
			fcntl( pd.iStdOut, F_SETFL, fcntl(pd.iStdOut,F_GETFL,0 )&(~O_NONBLOCK) );
		if( !bStdErrEos )
			fcntl( pd.iStdErr, F_SETFL, fcntl(pd.iStdErr,F_GETFL,0 )&(~O_NONBLOCK) );
	}
	else
	{
		if( !bStdOutEos )
			fcntl( pd.iStdOut, F_SETFL, fcntl( pd.iStdOut, F_GETFL, 0 )|O_NONBLOCK );
		if( !bStdErrEos )
			fcntl( pd.iStdErr, F_SETFL, fcntl( pd.iStdErr, F_GETFL, 0 )|O_NONBLOCK );
	}
	this->bBlocking = bBlocking;
#endif
}

void Bu::Process::setSize( Bu::size )
{
}

Bu::size Bu::Process::getBlockSize() const
{
	return 0;
}

Bu::size Bu::Process::getSize() const
{
	return 0;
}

Bu::String Bu::Process::getLocation() const
{
	return "";
}

void Bu::Process::select( bool &bStdOut, bool &bStdErr )
{
#ifdef WIN32
#else
	fd_set rfds;
	FD_ZERO( &rfds );
	if( !bStdOutEos )
		FD_SET( pd.iStdOut, &rfds );
	if( !bStdErrEos )
		FD_SET( pd.iStdErr, &rfds );
	if( ::bu_select( pd.iStdErr+1, &rfds, NULL, NULL, NULL ) < 0 )	
		throw Bu::ExceptionBase( strerror( errno ) );

	if( FD_ISSET( pd.iStdOut, &rfds ) )
		bStdOut = true;
	else
		bStdOut = false;

	if( FD_ISSET( pd.iStdErr, &rfds ) )
		bStdErr = true;
	else
		bStdErr = false;
#endif
}

bool Bu::Process::isRunning()
{
#ifdef WIN32
	DWORD lExitCode;
	GetExitCodeProcess( pd.hProcess, &lExitCode );
	if( lExitCode != STILL_ACTIVE )
		checkClose();
#else
	if( waitpid( pd.iPid, NULL, WNOHANG ) == pd.iPid )
		checkClose();
#endif
	return isOpen();
}

void Bu::Process::ignoreStdErr()
{
#ifdef WIN32
#else
	if( pd.iStdErr == -1 )
		return;
	::close( pd.iStdErr );
	pd.iStdErr = -1;
	bStdErrEos = true;
#endif
}

pid_t Bu::Process::getPid()
{
#ifdef WIN32
	return 0;
#else
	return pd.iPid;
#endif
}

bool Bu::Process::childExited()
{
#ifdef WIN32
	return pd.hProcess != NULL;
#else
	return WIFEXITED( iProcStatus );
#endif
}

int Bu::Process::childExitStatus()
{
#ifdef WIN32
	return iProcStatus;
#else
	return WEXITSTATUS( iProcStatus );
#endif
}

bool Bu::Process::childSignaled()
{
#ifdef WIN32
	return false;
#else
	return WIFSIGNALED( iProcStatus );
#endif
}

int Bu::Process::childSignal()
{
#ifdef WIN32
	return 0;
#else
	return WTERMSIG( iProcStatus );
#endif
}

bool Bu::Process::childCoreDumped()
{
#ifdef WIN32
	return false;
#else
	return WCOREDUMP( iProcStatus );
#endif
}

void Bu::Process::checkClose()
{
	if( bStdOutEos && bStdErrEos )
	{
		close();
	}
}
