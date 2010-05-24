/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_STREAM_H
#define BU_STREAM_H

#include <stdint.h>
#include <stdio.h>

#include "bu/fstring.h"

namespace Bu
{
	/**
	 * The basis for a completely general data transport mechanism.  Anything
	 * that inherits from this should provide at least the basic read and/or
	 * write functions, and very probably the close function.  Any functions
	 * that aren't supported should throw an exception if called.
	 *
	 * The constructor of a child class should pretty much universally be used
	 * to open the stream.  I can't think of anything that should require an
	 * exception.
	 *@ingroup Streams
	 */
	class Stream
	{
	public:
		Stream();
		virtual ~Stream();

		/**
		 * Close the stream.
		 */
		virtual void close() = 0;

		/**
		 * Read data from the stream into a buffer.
		 *@param pBuf (void *) Buffer which will be filled.
		 *@param nBytes (size_t) Max data to read.
		 *@returns (size_t) Amount of data read.
		 */
		virtual size_t read( void *pBuf, size_t nBytes ) = 0;

		/**
		 * Attempts to read a complete line from the stream.  This will stop
		 * reading when it has reached the end of the stream, or runs out of
		 * data in a non-blocking stream.
		 *@returns The line read, not including newline character.
		 */
		virtual Bu::FString readLine();

		/**
		 * Write data to the stream.
		 *@param pBuf (const void *) The data to be written.
		 *@param nBytes (size_t) Amount of data to write from pBuf.
		 *@returns (size_t) Amount of data actually written.
		 */
		virtual size_t write( const void *pBuf, size_t nBytes ) = 0;

		virtual size_t write( const Bu::FString &sBuf );

		/**
		 * Get the current position in the stream.
		 *@returns (long) The current position in the stream.
		 */
		virtual long tell() = 0;

		/**
		 * Seek to a position in the stream relative to the current position.
		 *@param offset (long) Offset from current position to seek to.
		 */
		virtual void seek( long offset ) = 0;

		/**
		 * Set position in the stream relative to the start of the stream.
		 *@param pos (long) The position.
		 */
		virtual void setPos( long pos ) = 0;

		/**
		 * Set position in the stream relative to the end of the stream.
		 *@param pos (long) The position.
		 */
		virtual void setPosEnd( long pos ) = 0;

		/**
		 * Are we at the end of the stream?
		 *@returns (bool) Are we at the end of the stream?
		 */
		virtual bool isEos() = 0;

		/**
		 * Is the stream open?
		 *@returns (bool) Is the stream open?
		 */
		virtual bool isOpen() = 0;

		/**
		 * Flush any data still held in buffers.
		 */
		virtual void flush() = 0;

		/**
		 * In non-blocking streams this indicates if a read operation will
		 * return data at the moment or not.  In blocking streams this should
		 * return the same value as isEos().
		 */
		virtual bool canRead() = 0;

		/**
		 * In non-blocking streams this indicates if a write operation will
		 * actually write one or more bytes.  In some cases writing is not
		 * allowed (e.g. internal buffers are full) temporarilly.  In blocking
		 * streams this should return the same value as isWritable.
		 */
		virtual bool canWrite() = 0;

		/**
		 * Indicates if the stream is capable of read operations.  This does not
		 * indicate if such operations will return useful data, see canRead for
		 * that.
		 */
		virtual bool isReadable() = 0;

		/**
		 * Indicates if the stream is capable of write operations.  This does
		 * not indicate if such operations will succeed or fail, see canWrite
		 * for that.
		 */
		virtual bool isWritable() = 0;

		/**
		 * Indicates if the stream is capable of seek operations.  This is
		 * generally false for non-blocking streams.  Some buffered streams may
		 * support limited in-buffer seeking.
		 */
		virtual bool isSeekable() = 0;

		/**
		 * Are we currently set to block mode?
		 *@returns (bool)
		 */
		virtual bool isBlocking() = 0;

		/**
		 * Set stream to blocking or non-blocking mode.
		 *@param bBlocking (bool) Whether we should block or not.
		 */
		virtual void setBlocking( bool bBlocking=true ) = 0;

		/**
		 * Set the size of the stream, this does not apply to many types of
		 * streams.  For those that it does apply to, data will be added or
		 * removed from the end of the stream, but the content of the added
		 * data is undefined.
		 */
		virtual void setSize( long iSize ) = 0;

	public: // Filters


	private:

	};
}

#endif
