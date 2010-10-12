#include "bu/parser.h"
#include "bu/lexer.h"

#include "bu/sio.h"
using namespace Bu;

Bu::Parser::Parser()
{
}

Bu::Parser::~Parser()
{
}

void Bu::Parser::pushLexer( Lexer *pLex )
{
	sLexer.push( pLex );
}

void Bu::Parser::popLexer()
{
	delete sLexer.peekPop();
}

void Bu::Parser::parse()
{
	for(;;)
	{
		Bu::Lexer::Token *pToken = sLexer.peek()->nextToken();
		sio << sLexer.peek()->tokenToString( *pToken ) << sio.nl;
		if( pToken->iToken < 0 )
		{
			delete sLexer.peekPop();
			if( sLexer.isEmpty() )
			{
				delete pToken;
				return;
			}
		}
		delete pToken;
	}	
}

