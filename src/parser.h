#ifndef BU_PARSER_H
#define BU_PARSER_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/array.h"
#include "bu/hash.h"
#include "bu/signals.h"

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

		Lexer::Token *popToken();
		void pushToken( Lexer::Token *pTok );

		/**
		 * Execute a parse.
		 */
		void parse();

		void setRootNonTerminal( int iRoot );
		void setRootNonTerminal( const Bu::FString &sRoot );

		typedef Bu::Signal1<void, Parser &> Reduction;

		/**
		 * Represents a possible state, either a terminal or non-terminal symbol
		 * in a Production.
		 */
		class State
		{
		public:
			enum Type
			{
				typeTerminal,
				typeTerminalPush,
				typeNonTerminal,
				typeReduction
			};

			State( Type eType, int iIndex );
			virtual ~State();

		//private:
			Type eType;
			int iIndex;
		};

		typedef Bu::List<State> Production;

		class NonTerminal
		{
		public:
			NonTerminal();
			virtual ~NonTerminal();

			void addProduction( Production p );
			void setCanSkip();

//		private:
			typedef Bu::List<Production> ProductionList;
			ProductionList lProduction;
			bool bCanSkip;
		};

		int addNonTerminal( const Bu::FString &sName, NonTerminal &nt );
		int addNonTerminal( const Bu::FString &sName );
		void setNonTerminal( const Bu::FString &sName, NonTerminal &nt );
		int getNonTerminalId( const Bu::FString &sName );
		bool hasNonTerminal( const Bu::FString &sName );

		int addReduction( const Bu::FString &sName, const Reduction &r );
		int addReduction( const Bu::FString &sName );
		void setReduction( const Bu::FString &sName, const Reduction &r );
		int getReductionId( const Bu::FString &sName );
		bool hasReduction( const Bu::FString &sName );

	private:
		bool selectProduction( int iNt, Lexer::Token *ptCur );
		void advanceState();

	private:
		typedef Bu::List<Lexer *> LexerStack;
		typedef Bu::List<Lexer::Token *> TokenStack;
		typedef Bu::List<Production::const_iterator> StateStack;
		typedef Bu::Array<Reduction> ReductionArray;
		typedef Bu::Hash<Bu::FString,int> NameIndexHash;
		typedef Bu::Array<NonTerminal> NonTerminalArray;

		LexerStack sLexer;
		TokenStack sToken;
		StateStack sState;
		ReductionArray aReduction;
		NameIndexHash hReductionName;
		NonTerminalArray aNonTerminal;
		NameIndexHash hNonTerminalName;
		int iRootNonTerminal;
	};
Bu::Formatter &operator<<( Bu::Formatter &f, Bu::Parser::State::Type t );
Bu::Formatter &operator<<( Bu::Formatter &f, const Bu::Parser::State &s );
};


#endif
