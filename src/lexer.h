#ifndef BU_LEXER_H
#define BU_LEXER_H

#include "bu/variant.h"

namespace Bu
{
	class Stream;

	/**
	 * The base class for creating a lexical analyzer.  This is designed to work
	 * in tandem with the Bu::Parser class, which uses this to tokenize textual
	 * input.  It can be used by just about anything that cares about tokens
	 * more than raw input, though.
	 */
	class Lexer
	{
	public:
		Lexer();
		virtual ~Lexer();

		typedef int TokenType;

		class Token
		{
		public:
			Token();
			Token( TokenType iToken );

			template<class t>
			Token( TokenType iToken, const t &v ) :
				iToken( iToken ),
				vExtra( v )
			{
			}
			TokenType iToken;
			Bu::Variant vExtra;
		};

		virtual Token *nextToken()=0;

		virtual Bu::FString tokenToString( const Token &t );
	};
};

#endif
