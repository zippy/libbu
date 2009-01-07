/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_PARAM_PROC_H
#define BU_PARAM_PROC_H

#include <stdint.h>
#include "bu/list.h"
#include "bu/fstring.h"

namespace Bu
{
	class ParamProc
	{
	public:
		class ParamPtr
		{
		public:
			ParamPtr();
			ParamPtr( Bu::FString *str );
			ParamPtr( uint64_t    *uint64  );
			ParamPtr( uint32_t    *uint32  );
			ParamPtr( uint16_t    *uint16  );
			ParamPtr( uint8_t     *uint8   );
			ParamPtr( int64_t     *int64   );
			ParamPtr( int32_t     *int32   );
			ParamPtr( int16_t     *int16   );
			ParamPtr( int8_t      *int8    );
			ParamPtr( float       *float32 );
			ParamPtr( double      *float64 );
			ParamPtr( long double *float96 );
			ParamPtr( bool        *bln     );

			enum
			{
				vtunset,
				vtstr,
				vtuint64,
				vtuint32,
				vtuint16,
				vtuint8,
				vtint64,
				vtint32,
				vtint16,
				vtint8,
				vtfloat32,
				vtfloat64,
				vtfloat96,
				vtbln,
			};
			ParamPtr &operator=( ParamPtr &ptr );
			ParamPtr &operator=( const char *str );

			bool isSet();

		private:
			int type;
			union
			{
				Bu::FString *str;
				uint64_t *uint64;
				uint32_t *uint32;
				uint16_t *uint16;
				uint8_t *uint8;
				int64_t *int64;
				int32_t *int32;
				int16_t *int16;
				int8_t *int8;
				float *float32;
				double *float64;
				long double *float96;
				bool *bln;
			} val;
		};

		typedef int (ParamProc::*Proc)( int, char *[] );

		typedef struct ArgSpec
		{
			uint8_t nFlags;
			Bu::FString sWord;
			char cChar;
			Proc proc;
			ParamProc::ParamPtr val;
			Bu::FString sExtra;
			Bu::FString sDesc;
			Bu::FString sValue;
		} ArgSpec;

	public:
		ParamProc();
		virtual ~ParamProc();
		
		void addParam( const char *lpWord, char cChar, Proc proc, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( const char *lpWord, char cChar, Proc proc,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( const char *lpWord, char cChar, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);

		void addParam( const char *lpWord, Proc proc, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( const char *lpWord, Proc proc,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( const char *lpWord, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		
		void addParam( char cChar, Proc proc, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( char cChar, Proc proc,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);
		void addParam( char cChar, ParamPtr val,
			const char *lpDesc=NULL, const char *lpExtra=NULL,
			const char *lpValue=NULL
			);

		void process( int argc, char *argv[] );
		void addHelpBanner( const char *sHelpBanner );

	private:
		ArgSpec *checkWord( const char *arg );
		ArgSpec *checkLetr( const char arg );

	public:
		virtual int cmdParam( int argc, char *argv[] );
		virtual int unknownParam( int argc, char *argv[] );
		virtual int help( int argc, char *argv[] );

	private:
		typedef struct Banner
		{
			Bu::FString sBanner;
			ArgSpec *pBefore;
		} Banner;
		Bu::List<Banner *> lBan;
		Bu::List<ArgSpec *> lArg;
	};
}

#define mkproc( cls ) static_cast<int (Bu::ParamProc::*)( int, char *[])>(&cls)

#endif
