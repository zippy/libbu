/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#define FUSE_USE_VERSION 26

#include <fuse.h>

#include <string.h>
#include <errno.h>

#include <bu/file.h>
#include <bu/myriadfs.h>
#include <bu/myriadstream.h>
#include <bu/hash.h>

Bu::File *pF = NULL;
Bu::MyriadFs *pFs = NULL;

typedef Bu::Hash<int64_t, Bu::MyriadStream> FileHash;
FileHash hOpenFiles;
int64_t iNextFileId = 0;

extern "C" {
	static int myriadfs_getattr( const char *sPath, struct stat *stbuf )
	{
		try
		{
			printf("myriadfs_getattr: Statting file: %s\n", sPath );
			Bu::MyriadFs::Stat st;
			pFs->stat( sPath, st );
			stbuf->st_ino = st.iNode;
			stbuf->st_mode = st.uPerms;
			stbuf->st_nlink = st.iLinks;
			stbuf->st_uid = st.iUser;
			stbuf->st_gid = st.iGroup;
			stbuf->st_rdev = Bu::MyriadFs::devToSys( st.uDev );
			stbuf->st_size = st.iSize;
			stbuf->st_blocks = 8;
			stbuf->st_atime = st.iATime;
			stbuf->st_mtime = st.iMTime;
			stbuf->st_ctime = st.iCTime;
			return 0;
		}
		catch(...)
		{
			return -ENOENT;
		}
	}

	static int myriadfs_readdir( const char *sPath, void *buf,
			fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi )
	{
		fprintf( stderr, "Reading dir...\n");
		Bu::MyriadFs::Dir lDir = pFs->readDir( sPath );
		filler( buf, ".", NULL, 0 );
		filler( buf, "..", NULL, 0 );
		for( Bu::MyriadFs::Dir::iterator i = lDir.begin(); i; i++ )
		{
			fprintf( stderr, "Adding file: '%s'\n", (*i).sName.getStr() );
			filler( buf, (*i).sName.getStr(), NULL, 0 );
		}

		return 0;
	}

	static int myriadfs_mkdir( const char *sPath, mode_t uMode )
	{
		pFs->mkDir( sPath, uMode );
		return 0;
	}

	static int myriadfs_open( const char *sPath, struct fuse_file_info *fi )
	{
		try
		{
			Bu::MyriadStream ms = pFs->open( sPath, 0 );
			fi->fh = iNextFileId;
			hOpenFiles.insert( iNextFileId++, ms );
			return 0;
		}
		catch(...)
		{
			return -EACCES;
		}
	}

	static int myriadfs_read( const char *sPath, char *buf, size_t iSize,
			off_t iOffset, struct fuse_file_info *fi )
	{
		Bu::MyriadStream &ms = hOpenFiles.get( fi->fh );
		ms.setPos( iOffset );
		return ms.read( buf, iSize );
	}

	static int myriadfs_write( const char *sPath, const char *buf, size_t iSize,
			off_t iOffset, struct fuse_file_info *fi )
	{
		Bu::MyriadStream &ms = hOpenFiles.get( fi->fh );
		ms.setPos( iOffset );
		return ms.write( buf, iSize );
	}

	static int myriadfs_create( const char *sPath, mode_t uPerms,
			struct fuse_file_info *fi )
	{
		try
		{
			Bu::MyriadStream ms = pFs->open( sPath, 0 );
			fi->fh = iNextFileId;
			hOpenFiles.insert( iNextFileId++, ms );
			return 0;
		}
		catch(...)
		{
			return -EACCES;
		}
	}
	
	static int myriadfs_mknod( const char *sPath, mode_t uPerms, dev_t Dev )
	{
		try
		{
			pFs->create( sPath, uPerms, Bu::MyriadFs::sysToDev( Dev ) );
			return 0;
		}
		catch(...)
		{
			return -EACCES;
		}
	}

	static int myriadfs_release( const char *sPath, struct fuse_file_info *fi )
	{
		hOpenFiles.erase( fi->fh );

		return 0;
	}

	static int myriadfs_utimens( const char *sPath,
			const struct timespec tv[2] )
	{
		pFs->setTimes( sPath, tv[0].tv_sec, tv[1].tv_sec );
		return 0;
	}

	static struct fuse_operations myriadfs_oper;

	int main( int argc, char *argv[] )
	{
		pF = new Bu::File("store.myr", Bu::File::Read|Bu::File::Write|Bu::File::Create );
		pFs = new Bu::MyriadFs( *pF, 512 );
		memset( &myriadfs_oper, sizeof(myriadfs_oper), 0 );
		myriadfs_oper.getattr    = myriadfs_getattr;
		myriadfs_oper.readdir    = myriadfs_readdir;
		myriadfs_oper.mkdir		 = myriadfs_mkdir;
		myriadfs_oper.open       = myriadfs_open;
		myriadfs_oper.read       = myriadfs_read;
		myriadfs_oper.write      = myriadfs_write;
		myriadfs_oper.create	 = myriadfs_create;
		myriadfs_oper.mknod		 = myriadfs_mknod;
		myriadfs_oper.release	 = myriadfs_release;
		myriadfs_oper.utimens	 = myriadfs_utimens;
		printf("Starting fuse_main.\n");
		int iRet = fuse_main( argc, argv, &myriadfs_oper, NULL );
		printf("Done with fuse_main.\n");
		delete pFs;
		delete pF;
		return iRet;
	}
}
