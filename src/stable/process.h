/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PROCESS_H
#define BU_PROCESS_H

#include <stdint.h>
#include <sys/types.h>

#include "bu/stream.h"
#include "bu/string.h"

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
		enum Flags
		{
			None	=	0x00,
			StdOut	=	0x01,
			StdErr	=	0x02,
			Both	=	0x03
		};

	public:
		class Options
		{
		public:
			enum OptFlags
			{
				None	= 0x00,
				SetUid	= 0x01,
				SetGid	= 0x02,
			};

			Options() : eFlags( None ) {}

			int eFlags;
			int iUid;
			int iGid;
		};

		Process( Flags eFlags, const char *sName, char *const argv[] );
		Process( Flags eFlags, const char *sName, const char *argv, ...);
		Process( Flags eFlags, const Options &opt, const char *sName, char *const argv[] );
		Process( Flags eFlags, const Options &opt, const char *sName, const char *argv, ...);
		virtual ~Process();

		/**
		 * Waits until the process exits.  This blocks the caller until the
		 * child process terminates.
		 */
		void wait();

		virtual void close();
		virtual void closeStdIn();
		virtual void closeStdOut();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
		virtual Bu::size readErr( void *pBuf, Bu::size nBytes );
		virtual Bu::size write( const void *pBuf, Bu::size nBytes );
		using Stream::write;

		virtual Bu::size tell();
		virtual void seek( Bu::size offset );
		virtual void setPos( Bu::size pos );
		virtual void setPosEnd( Bu::size pos );
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

		virtual void setSize( Bu::size iSize );

		virtual size getBlockSize() const;
		virtual size getSize() const;
		virtual Bu::String getLocation() const;

		void select( bool &bStdOut, bool &bStdErr );

		bool isRunning();
		void ignoreStdErr();

		/**
		 * Returns the pid of the child process, or zero if there is no
		 * currently running child.  Note that a read operation must be
		 * performed in order to discover that the child has ended.
		 */
		pid_t getPid();

		/**
		 * Returns true if the child exited normally (by calling exit or
		 * returning from main).
		 */
		bool childExited();

		/**
		 * Returns the 8 bit integer value returned from the child program if
		 * childExited returned true.
		 */
		int childExitStatus();

		/**
		 * Returns true if the child exited because of a signal.
		 */
		bool childSignaled();

		/**
		 * Returns the signal ID if the childSignaled return true.
		 */
		int childSignal();

		/**
		 * Returns true if the child left a core dump behind when it exited.
		 */
		bool childCoreDumped();

	private:
		class ProcData
		{
		public:
			ProcData();
#ifdef WIN32
			void *hStdIn;
			void *hStdOut;
			void *hStdErr;
			void *hProcess;
#else
			int iStdIn;
			int iStdOut;
			int iStdErr;
			pid_t iPid;
#endif
		};
		ProcData pd;
		int iProcStatus;
		bool bBlocking;
		bool bStdOutEos;
		bool bStdErrEos;

		void gexec( Flags eFlags, const char *sName, char *const argv[] );
		void checkClose();
		Options opt;
	};
}

#endif
