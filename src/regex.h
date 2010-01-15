#ifndef BU_REG_EX_H
#define BU_REG_EX_H

#include "bu/fstring.h"

#include <stdint.h>

namespace Bu
{
	class RegEx
	{
	public:
		RegEx();
		RegEx( const Bu::FString &sSrc );
		virtual ~RegEx();

		void compile( const Bu::FString &sSrc );
		int getNumSubStrings();
		bool execute( const Bu::FString &sSrc );
		void getSubStringRange( int nIndex, int &iStart, int &iEnd );
		Bu::FString getSubString( int nIndex );
		const Bu::FString &getSource()
		{
			return sSrc;
		}

	private:
		Bu::FString sSrc;
		Bu::FString sTest;
		void *pRegEx;
		bool bCompiled;
		int nSubStr;
		void *paSubStr;
	};
};

#endif