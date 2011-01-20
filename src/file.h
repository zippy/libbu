/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FILE_H
#define BU_FILE_H

#include <stdint.h>
#include <sys/types.h>

#include "bu/stream.h"
#include "bu/string.h"
#include "bu/exceptionbase.h"

namespace Bu
{
	subExceptionDecl( FileException );

	/**
	 * A file stream.
	 *@ingroup Streams
	 */
	class File : public Bu::Stream
	{
	public:
		File( const Bu::String &sName, int iFlags );
		File( int fd );
		virtual ~File();

		virtual void close();
		virtual Bu::size read( void *pBuf, Bu::size nBytes );
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

		enum {
			// Flags
			Read		= 0x01,	///< Open file for reading
			Write		= 0x02, ///< Open file for writing
			Create		= 0x04, ///< Create file if it doesn't exist
			Truncate	= 0x08, ///< Truncate file if it does exist
			Append		= 0x10, ///< Always append on every write
			NonBlock	= 0x20, ///< Open file in non-blocking mode
			Exclusive	= 0x44, ///< Create file, if it exists then fail

			// Helpful mixes
			ReadWrite	= 0x03, ///< Open for reading and writing
			WriteNew	= 0x0E	///< Create a file (or truncate) for writing.
								/// Same as Write|Create|Truncate
		};

		virtual void setSize( Bu::size iSize );

		virtual size getSize() const;
		virtual size getBlockSize() const;
		virtual Bu::String getLocation() const;

		/**
		 * Create a temp file and return its handle.  The file is opened 
		 * Read/Write.
		 *@param sName (Bu::String) Give in the form: "/tmp/tmpfileXXXXXXXX"
		 *		It will alter your (sName) setting the 'X's to random
		 *		characters.
		 *@returns (Bu::File) A file object representing your temp file.
		 */
		static Bu::File tempFile( Bu::String &sName );

#ifndef WIN32
		/**
		 * Change the file access permissions.
		 *@param t (mode_t) The new file access permissions.
		 */
		void chmod( mode_t t );
#endif

	private:
		int getPosixFlags( int iFlags );

	private:
		int fd;
		bool bEos;
	};
}

#endif
