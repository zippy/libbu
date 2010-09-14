/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archive.h"
#include "bu/stream.h"
#include "bu/archival.h"

#include "bu/sio.h"

Bu::Archive::Archive( Stream &rStream, bool bLoading ) :
	bLoading( bLoading ),
	rStream( rStream ),
	nNextID( 1 )
{
}

Bu::Archive::~Archive()
{
}

void Bu::Archive::write( const void *pData, int32_t nSize )
{
	if( nSize == 0 || pData == NULL )
		return;

//	Bu::sio << "Writing starting at pos: " << rStream.tell() << " - "
//		<< Bu::sio.flush;

	rStream.write( (const char *)pData, nSize );
//
//	Bu::sio << rStream.tell() << " (" << nSize << "b)" << Bu::sio.nl;
}

void Bu::Archive::read( void *pData, int32_t nSize )
{
	if( nSize == 0 || pData == NULL )
		return;

	if( rStream.read( (char *)pData, nSize ) < nSize )
		throw Bu::ExceptionBase("Insufficient data to unarchive object.");
}

void Bu::Archive::close()
{
	rStream.close();
}

bool Bu::Archive::isLoading()
{
	return bLoading;
}

uint32_t Bu::Archive::getID( const void *ptr )
{
	if( hPtrID.has( (ptrdiff_t)ptr ) )
		return hPtrID.get( (ptrdiff_t)ptr );
	hPtrID.insert( (ptrdiff_t)ptr, nNextID );
	return nNextID++;
}

void Bu::Archive::assocPtrID( void **ptr, uint32_t id )
{
	if( hPtrID.has( id ) )
	{
		*ptr = (void *)hPtrID.get( id );
		return;
	}

	if( !hPtrDest.has( id ) )
		hPtrDest.insert( id, List<void **>() );
			
	hPtrDest[id].getValue().append( ptr );
}

void Bu::Archive::readID( const void *ptr, uint32_t id )
{
	hPtrID.insert( id, (ptrdiff_t)ptr );

	if( hPtrDest.has( id ) )
	{
		Bu::List<void **> &l = hPtrDest.get( id );
		for( Bu::List<void **>::iterator i = l.begin(); i != l.end(); i++ )
		{
			*(*i) = (void *)ptr;
		}

		hPtrDest.erase( id );
	}
}

