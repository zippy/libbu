#ifndef BU_OPT_PARSER_H
#define BU_OPT_PARSER_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/hash.h"
#include "bu/signals.h"
#include "bu/array.h"

namespace Bu
{
	typedef Bu::Array<Bu::FString> StrArray;
	class OptParser
	{
	public:
		typedef Signal1<int, StrArray> OptionSignal;
		class Option
		{
		public:
			Option();
			virtual ~Option();

			char cOpt;
			Bu::FString sOpt;
			Bu::FString sHelp;
			OptionSignal sUsed;
			bool bShortHasParams;
		};

	public:
		OptParser();
		virtual ~OptParser();

		void parse( int argc, char **argv );

		void addOption( const Option &opt );
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
