/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#define FUSE_USE_VERSION 26

#include <fuse.h>

#include <string.h>

extern "C" {
	static int myriadfs_getattr( const char *sPath, struct stat *stbuf )
	{

	}

	static int myriadfs_readdir( const char *sPath, void *buf,
			fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
	{
	}

	static int myriadfs_open( const char *sPath, struct fuse_file_info *fi )
	{
	}

	static int myriadfs_read( const char *sPath, char *buf, size_t iSize,
			off_t iOffset, struct fuse_file_info *fi )
	{
	}

	static struct fuse_operations myriadfs_oper;

	int main( int argc, char *argv[] )
	{
		memset( &myriadfs_oper, sizeof(myriadfs_oper), 0 );
		myriadfs_oper.getattr    = myriadfs_getattr;
		myriadfs_oper.readdir    = myriadfs_readdir;
		myriadfs_oper.open       = myriadfs_open;
		myriadfs_oper.read       = myriadfs_read;
		return fuse_main( argc, argv, &myriadfs_oper, NULL );
	}
}
