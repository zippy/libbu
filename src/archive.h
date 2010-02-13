/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ARCHIVE_H
#define BU_ARCHIVE_H

#include <stdint.h>
#include "bu/archivebase.h"
#include "bu/hash.h"
#include "bu/util.h"

namespace Bu
{
	class Archival;
	class Stream;

	/**
	 * Provides a framework for serialization of objects and primitives.  The
	 * archive will handle any basic primitive, a few special types, like char *
	 * strings, as well as STL classes and anything that inherits from the
	 * Archival class.  Each Archive operates on a Stream, so you can send the
	 * data using an Archive almost anywhere.
	 *
	 * In order to use an Archive to store something to a file, try something
	 * like:
	 *@code
     * File sOut("output", "wb"); // This is a stream subclass
	 * Archive ar( sOut, Archive::save );
	 * ar << myClass;
	 @endcode
	 * In this example myClass is any class that inherits from Archival.  When
	 * the storage operator is called, the Archival::archive() function in the
	 * myClass object is called with a reference to the Archive.  This can be
	 * handled in one of two ways:
	 *@code
	 * void MyClass::archive( Archive &ar )
	 * {
	 * 	ar && sName && nAge && sJob;
	 * }
	 @endcode
	 * Here we don't worry about weather we're loading or saving by using the
	 * smart && operator.  This allows us to write very consistent, very simple
	 * archive functions that really do a lot of work.  If we wanted to do
	 * something different in the case of loading or saving we would do:
	 *@code
	 * void MyClass::archive( Archive &ar )
	 * {
	 * 	if( ar.isLoading() )
	 * 	{
	 * 		ar >> sName >> nAge >> sJob;
	 * 	} else
	 * 	{
	 * 		ar << sName << nAge << sJob;
	 * 	}
	 * }
	 @endcode
	 * Archive currently does not provide facility to make fully portable
	 * archives.  For example, it will not convert between endianness for you,
	 * nor will it take into account differences between primitive sizes on
	 * different platforms.  This, at the moment, is up to the user to ensure.
	 * One way of dealing with the latter problem is to make sure and use
	 * explicit primitive types from the stdint.h header, i.e. int32_t.
	 */
	class Archive : public ArchiveBase
	{
	private:
		bool bLoading;
	public:
		bool isLoading();

		enum
		{
			load	=	true,
			save	=	false
		};
		
		Archive( Stream &rStream, bool bLoading );
		virtual ~Archive();
		virtual void close();

		virtual void write(const void *, int32_t);
		virtual void read(void *, int32_t);
		
		/**
		 * For storage, get an ID for the pointer to the object you're going to
		 * write.
		 */
		uint32_t getID( const void *ptr );

		/**
		 * For loading.  Assosiates an empty pointer with an id.  When you wind
		 * up loading an id reference to a pointer for an object that may or
		 * may not have loaded yet, call this with the id, if it has been loaded
		 * already, you'll immediately get a pointer, if not, it will write one
		 * for you when the time comes.
		 */
		void assocPtrID( void **ptr, uint32_t id );

		/**
		 * For loading.  Call this when you load an object that other things may
		 * have pointers to.  It will assosiate every pointer that's been
		 * registered with assocPtrID to the pointer passed in, and id passed
		 * in.  It will also set things up so future calls to assocPtrID will
		 * automatically succeed immediately.
		 */
		void readID( const void *ptr, uint32_t id );

	private:
		Stream &rStream;
		uint32_t nNextID;
		Hash<uint32_t,uint32_t> hPtrID;
		Hash<uint32_t,List<void **> > hPtrDest;
	};
}

#endif
