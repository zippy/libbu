/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_OPT_PARSER_H
#define BU_OPT_PARSER_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/hash.h"
#include "bu/signals.h"
#include "bu/array.h"
#include "bu/membuf.h"
#include "bu/formatter.h"

namespace Bu
{
	typedef Bu::Array<Bu::FString> StrArray;

	/**
	 * POSIX/Gnu style command line parser.  Handles long and short options in
	 * a variety of fun and useful ways, along with singal based callbacks and
	 * automatic variable setting.  It's pretty easy to use, and very flexible.
	 *
	 * OptParser supports it's own builtin help mechanism which automatically
	 * enumerates the available options and their help in a well formatted and
	 * easy to read way, automatically formatting your help text per option and
	 * allows for addition "help banners" which can be placed wherever you
	 * would like.
	 */
	class OptParser
	{
	private:
		class _ValueProxy
		{
		public:
			_ValueProxy();
			virtual ~_ValueProxy();

			virtual void setValue( const Bu::FString & )=0;
			virtual _ValueProxy *clone()=0;
		};

		template<typename ptype>
		class ValueProxy : public _ValueProxy
		{
		public:
			ValueProxy( ptype &v ) :
				v( v )
			{
			}

			virtual ~ValueProxy()
			{
			}

			virtual void setValue( const Bu::FString &sVal )
			{
				Bu::MemBuf mb( sVal );
				Bu::Formatter f( mb );
				f >> v;
			}
			
			virtual _ValueProxy *clone()
			{
				return new ValueProxy<ptype>( v );
			}

		private:
			ptype &v;
		};

	public:
		typedef Signal1<int, StrArray> OptionSignal;
		class Option
		{
		public:
			Option();
			Option( const Option &rSrc );
			virtual ~Option();

			char cOpt;
			Bu::FString sOpt;
			Bu::FString sHelp;
			OptionSignal sUsed;
			_ValueProxy *pProxy;
			Bu::FString sOverride;
			Bu::FString sHelpDefault;
		};
	
	private:
		typedef Bu::List<Option> OptionList;
		typedef Bu::Hash<char, Option *> ShortOptionHash;
		typedef Bu::Hash<Bu::FString, Option *> LongOptionHash;
		
		class Banner
		{
		public:
			Bu::FString sText;
			bool bFormatted;
			OptionList::const_iterator iAfter;
		};

		typedef Bu::List<Banner> BannerList;

	public:
		OptParser();
		virtual ~OptParser();

		void parse( int argc, char **argv );

		void addOption( const Option &opt );
		
		template<typename vtype>
		void addOption( vtype &var, char cOpt, const Bu::FString &sOpt,
				const Bu::FString &sHelp )
		{
			Option o;
			o.cOpt = cOpt;
			o.sOpt = sOpt;
			o.pProxy = new ValueProxy<vtype>( var );
			o.sHelp = sHelp;
			addOption( o );
		}
		
		template<typename vtype>
		void addOption( vtype &var, const Bu::FString &sOpt,
				const Bu::FString &sHelp )
		{
			addOption( var, '\0', sOpt, sHelp );
		}
		
		template<typename vtype>
		void addOption( vtype &var, char cOpt, const Bu::FString &sHelp )
		{
			addOption( var, cOpt, "", sHelp );
		}

		void addOption( OptionSignal sUsed, char cOpt, const Bu::FString &sOpt,
				const Bu::FString &sHelp )
		{
			Option o;
			o.cOpt = cOpt;
			o.sOpt = sOpt;
			o.sUsed = sUsed;
			o.sHelp = sHelp;
			addOption( o );
		}
		
		void addOption( OptionSignal sUsed, const Bu::FString &sOpt,
				const Bu::FString &sHelp )
		{
			addOption( sUsed, '\0', sOpt, sHelp );
		}
		
		void addOption( OptionSignal sUsed, char cOpt,
				const Bu::FString &sHelp )
		{
			addOption( sUsed, cOpt, "", sHelp );
		}

		void setOverride( char cOpt, const Bu::FString &sOverride );
		void setOverride( const Bu::FString &sOpt,
				const Bu::FString &sOverride );

		void setHelpDefault( const Bu::FString &sOpt, const Bu::FString &sTxt );
		
		void addHelpOption( char c='h', const Bu::FString &s="help",
				const Bu::FString &sHelp="This help." );
		void addHelpBanner( const Bu::FString &sText, bool bFormatted=true );

		int optHelp( StrArray aParams );

		/**
		 * This function is called when an unrecognized option is found, the
		 * default behaviour is to print an error to stdout and exit( 1 ), if
		 * you want to do something different, just override this function.
		 * This is also called by default when something is found that hasn't
		 * been handled by an option, and isn't an option (starts with - or --).
		 * To change this behaviour call 
		 */
		virtual void optionError( const Bu::FString sOption );

		void setNonOption( OptionSignal sSignal );

	private:
		Bu::FString format( const Bu::FString &sIn, int iWidth, int iIndent );

		OptionList lOption;
		ShortOptionHash hsOption;
		LongOptionHash hlOption;
		BannerList lBanner;
		OptionSignal sNonOption;
	};
};

#endif
