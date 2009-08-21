/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sharedcore.h"

void hardlog(const char *fmt, ...)
{
	va_list ap;
	va_start( ap, fmt );

	FILE *fh = fopen("sharedcore.log", "ab");
	vfprintf( fh, fmt, ap );
	va_end( ap );
	fclose( fh );
}

int iDepth = 0;

void fncin( void *base, const char *fn, void *core, int *iRefCount, int params, ... )
{
	va_list ap;
	va_start( ap, params );

	FILE *fh = fopen("sharedcore.log", "ab");
	fprintf( fh, "%08X: ", base );
	for( int j = 0; j < iDepth; j++ )
		fprintf( fh, "  " );
	fprintf( fh, "%s(", fn );
	for( int j = 0; j < params; j++ )
		if( j == 0 )
			fprintf( fh, " %08X", va_arg( ap, void * ) );
		else
			fprintf( fh, ", %08X", va_arg( ap, void * ) );
	if( params > 0 )
		fprintf( fh, " )");
	else
		fprintf( fh, ")");

	fprintf( fh, " [%08X / %08X] (%d)\n", core, iRefCount, rc );
	va_end( ap );
	fclose( fh );

	iDepth++;
}

void fncout( void *base, const char *fn, void *core, int *iRefCount, int params, ... )
{
	va_list ap;
	va_start( ap, params );

	iDepth--;

	FILE *fh = fopen("sharedcore.log", "ab");
	fprintf( fh, "%08X: ", base );
	for( int j = 0; j < iDepth; j++ )
		fprintf( fh, "  " );
	fprintf( fh, "%s(", fn );
	for( int j = 0; j < params; j++ )
		if( j == 0 )
			fprintf( fh, " %08X", va_arg( ap, void * ) );
		else
			fprintf( fh, ", %08X", va_arg( ap, void * ) );
	if( params > 0 )
		fprintf( fh, " )");
	else
		fprintf( fh, ")");

	fprintf( fh, " [%08X / %08X] (%d)\n", core, iRefCount, rc );
	va_end( ap );
	fclose( fh );
}

