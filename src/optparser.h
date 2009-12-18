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
	class OptParser
	{
	public:
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
			bool bShortHasParams;
			_ValueProxy *pProxy;
			Bu::FString sOverride;
		};

	public:
		OptParser();
		virtual ~OptParser();

		void parse( int argc, char **argv );

		void addOption( const Option &opt );
		
		template<typename vtype>
		void addOption( char cOpt, const Bu::FString &sOpt, vtype &var,
				const Bu::FString &sHelp="", const Bu::FString &sOverride="" )
		{
			Option o;
			o.cOpt = cOpt;
			o.sOpt = sOpt;
			o.pProxy = new ValueProxy<vtype>( var );
			o.bShortHasParams = true;
			o.sHelp = sHelp;
			o.sOverride = sOverride;
			addOption( o );
		}
		
		void addHelpOption( char c, const Bu::FString &s, const Bu::FString &sHelp );

		int optHelp( StrArray aParams );

	private:
		Bu::FString format( const Bu::FString &sIn, int iWidth, int iIndent );

		typedef Bu::List<Option> OptionList;
		typedef Bu::Hash<char, Option *> ShortOptionHash;
		typedef Bu::Hash<Bu::FString, Option *> LongOptionHash;

		OptionList lOption;
		ShortOptionHash hsOption;
		LongOptionHash hlOption;
	};
};

#endif
