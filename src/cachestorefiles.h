/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CACHE_STORE_FILES_H
#define BU_CACHE_STORE_FILES_H

#include "bu/fstring.h"
#include "bu/file.h"
#include "bu/cachestore.h"
#include "bu/archive.h"
#include "bu/membuf.h"
#include "bu/formatter.h"
#include "bu/sio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

namespace Bu
{
	template<class keytype, class obtype>
	keytype __cacheGetKey( const obtype *pObj );

	template<class keytype, class obtype>
	obtype *__cacheStoreFilesAlloc( const keytype &key )
	{
		return new obtype();
	}

	template<class keytype, class obtype>
	void __cacheStoreFilesStore( Bu::Stream &s, obtype &rObj,
			const keytype & )
	{
		Bu::Archive ar( s, Bu::Archive::save );
		ar << rObj;
	}

	template<class keytype, class obtype>
	obtype *__cacheStoreFilesLoad( Bu::Stream &s, const keytype &key )
	{
		obtype *pObj = __cacheStoreFilesAlloc<keytype, obtype>( key );
		Bu::Archive ar( s, Bu::Archive::load );
		ar >> (*pObj);
		return pObj;
	}

	template<class keytype, class obtype>
	class CacheStoreFiles : public CacheStore<keytype, obtype>
	{
	public:
		CacheStoreFiles( const Bu::FString &sPrefix ) :
			sPrefix( sPrefix )
		{
			if( access( sPrefix.getStr(), W_OK|R_OK|X_OK ) )
			{
				mkdir( sPrefix.getStr(), 0755 );
			}
		}

		virtual ~CacheStoreFiles()
		{
		}
		
		virtual obtype *load( const keytype &key )
		{
			try
			{
				Bu::MemBuf mb;
				Bu::Formatter f( mb );
				f << sPrefix << "/" << key;
				Bu::File fIn( mb.getString(), Bu::File::Read );
				obtype *pOb = __cacheStoreFilesLoad<keytype, obtype>( fIn, key );
				return pOb;
			}
			catch( std::exception &e )
			{
				throw Bu::HashException( e.what() );
			}
		}

		virtual void unload( obtype *pObj, const keytype & )
		{
			delete pObj;
		}

		virtual keytype create( obtype *pSrc )
		{
			keytype key = __cacheGetKey<keytype, obtype>( pSrc );
			Bu::MemBuf mb;
			Bu::Formatter f( mb );
			f << sPrefix << "/" << key;

			Bu::File fTouch( mb.getString(), Bu::File::WriteNew );

			return key;
		}

		virtual void sync()
		{
		}

		virtual void sync( obtype *pSrc, const keytype &key )
		{
			Bu::MemBuf mb;
			Bu::Formatter f( mb );
			f << sPrefix << "/" << key;

			Bu::File fOut( mb.getString(), Bu::File::WriteNew );
			__cacheStoreFilesStore<keytype, obtype>( fOut, *pSrc, key );
		}

		virtual void destroy( obtype *pObj, const keytype &key )
		{
			Bu::MemBuf mb;
			Bu::Formatter f( mb );
			f << sPrefix << "/" << key;

			unlink( mb.getString().getStr() );
			delete pObj;
		}

		virtual void destroy( const keytype &key )
		{
			Bu::MemBuf mb;
			Bu::Formatter f( mb );
			f << sPrefix << "/" << key;

			unlink( mb.getString().getStr() );
		}

		virtual bool has( const keytype &key )
		{
			Bu::MemBuf mb;
			Bu::Formatter f( mb );
			f << sPrefix << "/";
			Bu::FString sBase = mb.getString();
			f << key;

			if( sBase == mb.getString() )
				return false;

			return access( mb.getString().getStr(), F_OK ) == 0;
		}

		virtual Bu::List<keytype> getKeys()
		{
			DIR *dir = opendir( sPrefix.getStr() );
			struct dirent *de;
			Bu::List<keytype> lKeys;

			while( (de = readdir( dir ) ) )
			{
				if( de->d_type != DT_REG )
					continue;

				keytype tmp;
				Bu::MemBuf mb( de->d_name );
				Bu::Formatter f( mb );
				try
				{
					Fmt fm;
					fm.tokenize( false );
					f << fm;
					f >> tmp;
				}
				catch( Bu::ExceptionBase &e )
				{
					Bu::sio << "Parse error in dir-scan: " << e.what()
						<< Bu::sio.nl;
				}
				lKeys.append( tmp );
			}
			closedir( dir );

			return lKeys;
		}

		virtual int getSize()
		{
			DIR *dir = opendir( sPrefix.getStr() );
			struct dirent *de;
			int iCount = 0;

			while( (de = readdir( dir ) ) )
			{
				if( de->d_type != DT_REG )
					continue;

				iCount++;
			}
			closedir( dir );

			return iCount;
		}

	private:
		Bu::FString sPrefix;
	};

};

#endif
