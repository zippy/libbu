/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROCESS_H
#define BU_PROCESS_H

#include <stdint.h>
#include <sys/types.h>

#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 * Runs a program and attaches streams to it's stdin, stdout, and stderr.
	 * Reading from a Bu::Process will read from the program's standard output,
	 * writing to a Bu::Process will write to the program's standard input.
	 */
	class Process : public Bu::Stream
	{
	public:
		Process( const char *sName, char *const argv[] );
		Process( const char *sName, const char *argv, ...);
		virtual ~Process();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t readErr( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;

		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEos();
		virtual bool isOpen();

		virtual void flush();

		virtual bool canRead();
		virtual bool canWrite();

		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();

		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

	private:
		int iStdIn;
		int iStdOut;
		int iStdErr;
		pid_t iPid;

		void gexec( const char *sName, char *const argv[] );
	};
}

#endif
