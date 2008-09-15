/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_FILE_H
#define BU_FILE_H

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "bu/stream.h"
#include "bu/fstring.h"

namespace Bu
{
	/**
	 * A file stream.
	 *@ingroup Streams
	 */
	class File : public Bu::Stream
	{
	public:
		File( const char *sName, const char *sFlags );
		File( const Bu::FString &sName, const char *sFlags );
		File( int fd, const char *sFlags );
		virtual ~File();

		virtual void close();
		virtual size_t read( void *pBuf, size_t nBytes );
		virtual size_t write( const void *pBuf, size_t nBytes );
		using Stream::write;

		virtual long tell();
		virtual void seek( long offset );
		virtual void setPos( long pos );
		virtual void setPosEnd( long pos );
		virtual bool isEOS();
		virtual bool isOpen();

		virtual void flush();

		virtual bool canRead();
		virtual bool canWrite();

		virtual bool isReadable();
		virtual bool isWritable();
		virtual bool isSeekable();

		virtual bool isBlocking();
		virtual void setBlocking( bool bBlocking=true );

		/**
		 * Create a temp file and return its handle
		 *@param sName (Bu::FString) Give in the form: "/tmp/tmpfileXXXXXXXX"
		 *		It will alter your (sName) setting the 'X's to random
		 *		characters.
		 *@param sFlags (const char *) Standard file flags 'rb'... etc..
		 *@returns (Bu::File) A file object representing your temp file.
		 */
#ifndef WIN32
		inline static Bu::File tempFile( Bu::FString &sName, const char *sFlags )
		{
			int afh_d = mkstemp( sName.getStr() );

			return Bu::File( afh_d, sFlags );
		}

		/**
		 * Set the size of the file to (nSize). You can either grow or shrink
		 * 		the file.
		 *@param nSize (long) The new size of the file.
		 */
		void truncate( long nSize );

		/**
		 * Change the file access permissions.
		 *@param t (mode_t) The new file access permissions.
		 */
		void chmod( mode_t t );
#endif

	private:
		FILE *fh;

	};
}

#endif
