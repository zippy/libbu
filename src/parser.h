#ifndef BU_PARSER_H
#define BU_PARSER_H

#include "bu/list.h"
#include "bu/fstring.h"
#include "bu/lexer.h"

namespace Bu
{
	/**
	 * The base framework for a LR(1) grammar parser.  Provided a proper set of
	 * ParserStates this will prase any input the lexer can provide.
	 */
	class Parser
	{
	public:
		Parser();
		virtual ~Parser();

		/**
		 * When a Lexer is pushed onto the stack it becomes the source for
		 * future tokens read by the parser until it is popped off the stack.
		 * The Parser takes ownership of every Lexer pushed onto the stack,
		 * and will delete it when it is popped off the stack.
		 */
		void pushLexer( Lexer *pLex );

		/**
		 * Pop a lexer off the stack, and delete it.
		 */
		void popLexer();

		/**
		 * Execute a parse.
		 */
		void parse();

	private:
		typedef Bu::List<Lexer *> LexerStack;
		typedef Bu::List<Lexer::Token *> TokenStack;
		typedef Bu::List<State *> StateStack;
		LexerStack sLexer;
		TokenStack sToken;
		StateStack sState;
	};
};

#endif
