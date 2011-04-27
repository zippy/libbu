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

#define TRACE

extern "C" {
	static int myriadfs_getattr( const char *sPath, struct stat *stbuf )
	{
#ifdef TRACE
		printf("myriadfs_getattr(\"%s\", ... );\n", sPath );
#endif
		try
		{
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
#ifdef TRACE
		printf("myriadfs_readdir(\"%s\", ... );\n", sPath );
#endif
		Bu::MyriadFs::Dir lDir = pFs->readDir( sPath );
		filler( buf, ".", NULL, 0 );
		filler( buf, "..", NULL, 0 );
		for( Bu::MyriadFs::Dir::iterator i = lDir.begin(); i; i++ )
		{
			filler( buf, (*i).sName.getStr(), NULL, 0 );
		}

		return 0;
	}

	static int myriadfs_mkdir( const char *sPath, mode_t uMode )
	{
#ifdef TRACE
		printf("myriadfs_mkdir(\"%s\", 0%o );\n", sPath, uMode );
#endif
		pFs->mkDir( sPath, uMode );
		return 0;
	}

	static int myriadfs_open( const char *sPath, struct fuse_file_info *fi )
	{
#ifdef TRACE
		printf("myriadfs_open(\"%s\", ... );\n", sPath );
#endif
		try
		{
			Bu::MyriadStream ms = pFs->open( sPath, 0 );
			fi->fh = iNextFileId;
			hOpenFiles.insert( iNextFileId++, ms );
//			printf("File '%s' opened, %d files open now.\n",
//				sPath, hOpenFiles.getSize() );
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
#ifdef TRACE
		printf("myriadfs_read(\"%s\", ..., %d, %d, ... );\n", sPath, iSize,
				iOffset );
#endif
		Bu::MyriadStream &ms = hOpenFiles.get( fi->fh );
		ms.setPos( iOffset );
		return ms.read( buf, iSize );
	}

	static int myriadfs_write( const char *sPath, const char *buf, size_t iSize,
			off_t iOffset, struct fuse_file_info *fi )
	{
#ifdef TRACE
		printf("myriadfs_write(\"%s\", ..., %d, %d, ... );\n", sPath, iSize,
				iOffset );
#endif
		Bu::MyriadStream &ms = hOpenFiles.get( fi->fh );
		ms.setPos( iOffset );
		return ms.write( buf, iSize );
	}

	static int myriadfs_create( const char *sPath, mode_t uPerms,
			struct fuse_file_info *fi )
	{
#ifdef TRACE
		printf("myriadfs_create(\"%s\", 0%o, ... );\n", sPath, uPerms );
#endif
		try
		{
			Bu::MyriadStream ms = pFs->open( sPath, 0, uPerms );
			fi->fh = iNextFileId;
			hOpenFiles.insert( iNextFileId++, ms );
//			printf("File '%s' created, %d files open now.\n",
//				sPath, hOpenFiles.getSize() );
			return 0;
		}
		catch(...)
		{
			return -EACCES;
		}
	}
	
	static int myriadfs_mknod( const char *sPath, mode_t uPerms, dev_t Dev )
	{
#ifdef TRACE
		printf("myriadfs_mknod(\"%s\", 0%o, %x );\n", sPath, uPerms, Dev );
#endif
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
#ifdef TRACE
		printf("myriadfs_release(\"%s\", ... );\n", sPath );
#endif
		hOpenFiles.erase( fi->fh );
//		printf("File '%s' released, %d files open now.\n",
//			sPath, hOpenFiles.getSize() );

		return 0;
	}

	static int myriadfs_utimens( const char *sPath,
			const struct timespec tv[2] )
	{
#ifdef TRACE
		printf("myriadfs_utimens(\"%s\", ... );\n", sPath );
#endif
		try
		{
			pFs->setTimes( sPath, tv[0].tv_sec, tv[1].tv_sec );
		}
		catch(...)
		{
			return -EACCES;
		}
		return 0;
	}

	static int myriadfs_unlink( const char *sPath )
	{
#ifdef TRACE
		printf("myriadfs_unlink(\"%s\");\n", sPath );
#endif
		try
		{
			pFs->unlink( sPath );
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -EACCES;
		}
		return 0;
	}

	static int myriadfs_symlink( const char *sTarget, const char *sPath )
	{
#ifdef TRACE
		printf("myriadfs_symlink(\"%s\", \"%s\");\n", sTarget, sPath );
#endif
		try
		{
			pFs->mkSymLink( sTarget, sPath );
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -EACCES;
		}
		return 0;
	}

	static int myriadfs_readlink( const char *sPath, char *sOut, size_t s )
	{
#ifdef TRACE
		printf("myriadfs_readlink(\"%s\", ... );\n", sPath );
#endif
		try
		{
			Bu::String sTrg = pFs->readSymLink( sPath );
			size_t iLen = (s-1>sTrg.getSize())?(sTrg.getSize()):(s-1);
			memcpy( sOut, sTrg.getStr(), iLen );
			sOut[iLen] = '\0';
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -EACCES;
		}
		return 0;
	}

	static int myriadfs_truncate( const char *sPath, off_t iSize )
	{
#ifdef TRACE
		printf("myriadfs_truncate(\"%s\", %d );\n", sPath, iSize );
#endif

		try
		{
			pFs->setFileSize( sPath, iSize );
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -ENOENT;
		}
		return 0;
	}

	static int myriadfs_link( const char *sTarget, const char *sPath )
	{
#ifdef TRACE
		printf("myriadfs_link(\"%s\", \"%s\");\n", sTarget, sPath );
#endif

		try
		{
			pFs->mkHardLink( sTarget, sPath );
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -EACCES;
		}
		return 0;
	}

	static int myriadfs_rename( const char *sFrom, const char *sTo )
	{
#ifdef TRACE
		printf("myriadfs_rename(\"%s\", \"%s\");\n", sFrom, sTo );
#endif

		try
		{
			pFs->rename( sFrom, sTo );
		}
		catch( Bu::MyriadFsException &e )
		{
			printf("MyriadFsException: %s\n", e.what() );
			return -EACCES;
		}
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
		myriadfs_oper.unlink	 = myriadfs_unlink;
		myriadfs_oper.rmdir		 = myriadfs_unlink;
		myriadfs_oper.symlink	 = myriadfs_symlink;
		myriadfs_oper.readlink	 = myriadfs_readlink;
		myriadfs_oper.truncate	 = myriadfs_truncate;
		myriadfs_oper.link		 = myriadfs_link;
		myriadfs_oper.rename	 = myriadfs_rename;
		printf("Starting fuse_main.\n");
		int iRet = fuse_main( argc, argv, &myriadfs_oper, NULL );
		printf("Done with fuse_main.\n");
		delete pFs;
		delete pF;
		return iRet;
	}
}
