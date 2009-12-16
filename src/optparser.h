#ifndef BU_OPT_PARSER_H
#define BU_OPT_PARSER_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/hash.h"

namespace Bu
{
	class OptParser
	{
	public:
		class Option
		{
		public:
			Option();
			virtual ~Option();

			char cOpt;
			Bu::FString sOpt;
			Bu::FString sDesc;
		};

	public:
		OptParser();
		virtual ~OptParser();

		void parse( int argc, char *argv[] );

		void addOption( const Option &opt );

		template<typename c>
		void callback( c *pObj, int (c::*fnc)( int, char *[] ) )
		{
			(pObj->*fnc)( 0, NULL );
		}

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
